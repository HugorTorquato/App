import requests
from conftest import BASE as BASE_URL

def test_create_building():
    payload = {
        "id": 0,
        "name": "Sunset Towers",
        "address": "123 Sunset Blvd",
        "total_floors": 15,
        "apartments" : []
    }

    response = requests.post(f"{BASE_URL}/Building/Create", json=payload)

    assert response.status_code == 201

def test_get_building():
    # first create
    payload = {
        "id": 1,
        "name": "Ocean View",
        "address": "456 Ocean Ave",
        "total_floors": 15,
        "apartments" : []
    }

    create_res = requests.post(f"{BASE_URL}/Building/Create", json=payload)

    assert create_res.status_code == 201

    print(f"Create response: {create_res.json()}")

    # Adjust to extract the id from the response JSON when returning DTO
    building_id = create_res.json()["id"]
    print(f"Created building with ID: {building_id}")

    # then read
    res = requests.get(f"{BASE_URL}/Building/GetById/{building_id}")

    assert res.status_code == 200
    data = res.json()

    assert data["name"] == "Ocean View"
    assert data["address"] == "456 Ocean Ave"
    assert data["total_floors"] == 15