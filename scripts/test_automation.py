import pytest
import subprocess
import os
import time
import cv2
import numpy as np
from pathlib import Path

BINARY_PATH = Path(__file__).parent.parent / "build" / "spatial_capture"
TEST_DIR = Path(__file__).parent.parent / "test_images"
OUTPUT_DIR = Path(__file__).parent.parent / "output_images"

def setup_test_images():
    """Generate mock test images."""
    TEST_DIR.mkdir(exist_ok=True)
    OUTPUT_DIR.mkdir(exist_ok=True)

    # Valid image with rectangle
    img = np.zeros((100, 100, 3), dtype=np.uint8)
    cv2.rectangle(img, (20, 20), (80, 80), (255, 255, 255), -1)
    cv2.imwrite(str(TEST_DIR / "valid.png"), img)

    # Empty image (0x0)
    cv2.imwrite(str(TEST_DIR / "empty.png"), np.zeros((0, 0, 3), dtype=np.uint8))

    # Black image
    cv2.imwrite(str(TEST_DIR / "black.png"), np.zeros((100, 100, 3), dtype=np.uint8))

    # Corrupted (invalid file)
    with open(TEST_DIR / "corrupted.png", "wb") as f:
        f.write(b"not an image")

def teardown_test_images():
    """Clean up test images."""
    for file in TEST_DIR.glob("*"):
        file.unlink()
    TEST_DIR.rmdir()
    for file in OUTPUT_DIR.glob("*"):
        file.unlink()
    OUTPUT_DIR.rmdir()

@pytest.fixture(scope="session", autouse=True)
def manage_test_images():
    setup_test_images()
    yield
    teardown_test_images()

def run_binary(input_path, output_path):
    """Run the C++ binary and return result."""
    start_time = time.time()
    try:
        result = subprocess.run(
            [str(BINARY_PATH), str(input_path), str(output_path)],
            capture_output=True,
            text=True,
            timeout=10
        )
        elapsed = time.time() - start_time
        return result, elapsed
    except subprocess.TimeoutExpired:
        return None, time.time() - start_time
    except Exception as e:
        return None, time.time() - start_time

@pytest.mark.parametrize("image_name", ["valid.png", "empty.png", "black.png", "corrupted.png"])
def test_spatial_capture(image_name):
    """Test the spatial capture binary on various images."""
    input_path = TEST_DIR / image_name
    output_path = OUTPUT_DIR / f"output_{image_name}"

    result, elapsed = run_binary(input_path, output_path)

    # Log results
    print(f"Image: {image_name}, Time: {elapsed:.2f}s")
    if result:
        print(f"Return code: {result.returncode}")
        print(f"Stdout: {result.stdout}")
        print(f"Stderr: {result.stderr}")
    else:
        print("Binary crashed or timed out")

    # Assert no crash (return code 0 for valid, but handle others)
    if image_name == "valid.png":
        assert result is not None and result.returncode == 0, f"Failed on valid image: {result.stderr if result else 'Crash'}"
    # For others, just check it doesn't crash the system, but may return non-zero

    assert elapsed < 5, f"Too slow: {elapsed}s"  # Arbitrary threshold