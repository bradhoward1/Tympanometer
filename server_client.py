import requests


server_name = "http://127.0.0.1:5000"


def data():
    data = {"subject": 12,
    		"value": 103}
    r = requests.post(server_name+"/api/add_data", json=data)
    print(r.text)


if __name__ == '__main__':
    data()
