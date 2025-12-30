import requests
from conftest import BASE as BASE_URL

def test_create_resident():
    payload = {
        "id": 0,
        "name": "Hugo Torquato",
        "phone": "555-0100",
        "apartment_number": "303"
    }

    response = requests.post(f"{BASE_URL}/residents/Create", json=payload)

    assert response.status_code == 201

    # TODO:
    # - create route should return DTO instead of int id

    # data = response.json()

    # print(data)
    # assert data["id"] >= 0

    # assert data["name"] == "Hugo Torquato"
    # assert data["apartment_number"] == "303"


def test_get_resident():
    # first create
    payload = {
        "id": 1,
        "name": "Jane Doe",
        "phone": "555-8888",
        "apartment_number": "101"
    }

    create_res = requests.post(f"{BASE_URL}/residents/Create", json=payload)

    assert create_res.status_code == 201

    print(f"Create response: {create_res.json()}")

    # Adjust to extract the id from the response JSON when returning DTO
    resident_id = create_res.json()["id"]
    print(f"Created resident with ID: {resident_id}")

    # then read
    res = requests.get(f"{BASE_URL}/residents/GetById/{resident_id}")

    assert res.status_code == 200
    data = res.json()

    assert data["name"] == "Jane Doe"
    assert data["phone"] == "555-8888"
    assert data["apartment_number"] == "101"