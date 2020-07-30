# server.py

from flask import Flask, request, jsonify
from datetime import datetime
from pymodm import connect, MongoModel, fields
import PIL

connect("mongodb+srv://brad_howard:@cluster0-lucsp.mongodb.net"
        "/Tympanometer?retryWrites=true&w=majority")


app = Flask(__name__)


class SendData(MongoModel):
    subject = fields.CharField(primary_key=True)
    values = fields.ListField()


def __init__():
    print("Server is on.")


def check_keys(in_dict):
    my_keys = list(in_dict.keys())
    return my_keys


def add_data(in_dict):
    new_info = SendData()
    keys = check_keys(in_dict)
    for key in keys:
        if key == "subject":
            new_info.subject = in_dict[key]
        elif key == "value":
            new_info.values = in_dict[key]
    new_info.save()
    return True


@app.route("/api/add_data", methods=["POST"])
def post_info():
    in_dict = request.get_json()
    add_data(in_dict)
    return "Good post made to server", 200


if __name__ == '__main__':
    __init__()
    app.run()
