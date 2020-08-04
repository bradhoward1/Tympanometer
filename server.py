# server.py

from flask import Flask, request, jsonify
from datetime import datetime
from pymodm import connect, MongoModel, fields
import PIL

connect("mongodb+srv://brad_howard:saxman98@cluster0-lucsp.mongodb.net"
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
    # for key in keys:
    #     if key == "subject":
    #         new_info.subject = in_dict[key]
    #         new_info.save()
    #     elif key == "value_1":
    #         new_info.values = [in_dict[key]]
    #     elif key == "value_2":
    #         new_info = SendData.objects.raw({"_id": "Values"})
    #         new_info.update({"$push": {"values":
    #                         in_dict[key]}})
    #     elif key == "value_3":
    #         new_info = SendData.objects.raw({"_id": "Values"})
    #         new_info.update({"$push": {"values":
    #                         in_dict[key]}})
    #     elif key == "value_4":
    #         new_info = SendData.objects.raw({"_id": "Values"})
    #         new_info.update({"$push": {"values":
    #                         in_dict[key]}})
    #     elif key == "value_5":
    #         new_info = SendData.objects.raw({"_id": "Values"})
    #         new_info.update({"$push": {"values":
    #                         in_dict[key]}})
    #     elif key == "value_6":
    #         new_info = SendData.objects.raw({"_id": "Values"})
    #         new_info.update({"$push": {"values":
    #                         in_dict[key]}})
    #     elif key == "value_7":
    #         new_info = SendData.objects.raw({"_id": "Values"})
    #         new_info.update({"$push": {"values":
    #                         in_dict[key]}})
    #     elif key == "value_8":
    #         new_info = SendData.objects.raw({"_id": "Values"})
    #         new_info.update({"$push": {"values":
    #                         in_dict[key]}})
    #     elif key == "value_9":
    #         new_info = SendData.objects.raw({"_id": "Values"})
    #         new_info.update({"$push": {"values":
    #                         in_dict[key]}})
    #     elif key == "value_10":
    #         new_info = SendData.objects.raw({"_id": "Values"})
    #         new_info.update({"$push": {"values":
    #                         in_dict[key]}})
    #     elif key == "value_11":
    #         new_info = SendData.objects.raw({"_id": "Values"})
    #         new_info.update({"$push": {"values":
    #                         in_dict[key]}})
    #     elif key == "value_12":
    #         new_info = SendData.objects.raw({"_id": "Values"})
    #         new_info.update({"$push": {"values":
    #                         in_dict[key]}})
    #     elif key == "value_13":
    #         new_info = SendData.objects.raw({"_id": "Values"})
    #         new_info.update({"$push": {"values":
    #                         in_dict[key]}})
    #     elif key == "value_14":
    #         new_info = SendData.objects.raw({"_id": "Values"})
    #         new_info.update({"$push": {"values":
    #                         in_dict[key]}})
    #     elif key == "value_15":
    #         new_info = SendData.objects.raw({"_id": "Values"})
    #         new_info.update({"$push": {"values":
    #                         in_dict[key]}})
    # return True
    try:
        new_info = SendData.objects.raw({"_id": in_dict["subject"]})
        add_vals(in_dict)
    except:
        new_info.subject = in_dict["subject"]
        new_info.save()
        add_new_vals(in_dict)



def add_vals(in_dict):
    pass


def add_new_vals(in_dict):
    pass


@app.route("/api/add_data", methods=["POST"])
def post_info():
    in_dict = request.get_json()
    add_data(in_dict)
    return "Good post made to server", 200


if __name__ == '__main__':
    __init__()
    app.run(host="vcm-15218.vm.duke.edu")
