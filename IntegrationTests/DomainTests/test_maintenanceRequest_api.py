import requests
from conftest import BASE as BASE_URL

def test_create_maintenance_request():
    payload = {
        "id": 0,
        "resident_id": 1,
        "apartment_id": 1,
        "description": "Leaky faucet in kitchen",
        "status": 0, # "Open",
        "priority": 1
    }

    response = requests.post(f"{BASE_URL}/MaintenanceRequest/Create", json=payload)

    assert response.status_code == 201

def test_get_maintenance_request():
    # first create
    payload = {
        "id": 1,
        "resident_id": 2,
        "apartment_id": 2,
        "description": "Broken heater",
        "status": 1, # "In Progress"
        "priority": 1
    }

    create_res = requests.post(f"{BASE_URL}/MaintenanceRequest/Create", json=payload)

    assert create_res.status_code == 201

    print(f"Create response: {create_res.json()}")

    # Adjust to extract the id from the response JSON when returning DTO
    request_id = create_res.json()["id"]
    print(f"Created maintenance request with ID: {request_id}")

    # then read
    res = requests.get(f"{BASE_URL}/MaintenanceRequest/GetById/{request_id}")

    assert res.status_code == 200
    data = res.json()

    assert data["resident_id"] == 2
    assert data["description"] == "Broken heater"
    assert data["status"] == 1  # "In Progress"