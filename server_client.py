import requests


server_name = "http://127.0.0.1:5000"


def data():
    data = {"subject": "4",
            "value_1": "1",
            "value_2": "2",
            "value_3": "3",
            "value_4": "4",
            "value_5": "5",
            "value_6": "6",
            "value_7": "7",
            "value_8": "8",
            "value_9": "9",
            "value_10": "10",
            "value_11": "11",
            "value_12": "12",
            "value_13": "13",
            "value_14": "14",
            "value_15": "15"
            }
    r = requests.post(server_name+"/api/add_data", json=data)
    print(r.text)


if __name__ == '__main__':
    data()
