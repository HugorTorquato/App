import time
import requests

BASE = "http://app_prod:8080"

def wait_for_api(timeout=60):
    start = time.time()
    while True:
        try:
            r = requests.get(f"{BASE}/health", timeout=2)
            if r.status_code == 200:
                return
        except Exception:
            pass
            print("API not healthy yet...")

        if time.time() - start > timeout:
            raise RuntimeError("API did not become healthy in time")

        time.sleep(1)

def pytest_sessionstart(session):
    print("Waiting for API to become healthy...")
    wait_for_api()
    print("API is healthy, starting tests...")

