import requests
from conftest import BASE as BASE_URL

def test_create_apartment():
    payload = {
        "id": 0,
        "number": "501",
        "area_m2": 120.5
    }

    response = requests.post(f"{BASE_URL}/Apartment/Create", json=payload)

    assert response.status_code == 201

def test_get_apartment():
    # first create
    payload = {
        "id": 1,
        "number": "303",
        "area_m2": 900.5
    }

    create_res = requests.post(f"{BASE_URL}/Apartment/Create", json=payload)

    assert create_res.status_code == 201

    print(f"Create response: {create_res.json()}")

    # Adjust to extract the id from the response JSON when returning DTO
    apartment_id = create_res.json()["id"]
    print(f"Created apartment with ID: {apartment_id}")

    # then read
    res = requests.get(f"{BASE_URL}/Apartment/GetById/{apartment_id}")

    assert res.status_code == 200
    data = res.json()

    assert data["id"] == apartment_id
    assert data["number"] == "303"
    assert data["area_m2"] == 900.5