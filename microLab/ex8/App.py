
from flask import Flask, render_template, redirect, url_for, jsonify, request

app = Flask(__name__)
    
data: dict = {"status":"No data yet"}

@app.route('/')  # basic web route
def home():
    return "<h2>A server for Ex. 8 - MicroComputers Lab </h2>"
 

@app.route('/data', methods = ['GET', 'POST']) 
def data():
    global data
    if request.method == 'GET':
        # Ensure the `data` is always a valid dictionary
        if not data:
            response = jsonify({"message": "No data yet"})
        else:
            response = jsonify(data)
        return response
    ## POST request
    try:
        data = request.get_json()
    except Exception as e:
        return jsonify({"message" : f'Error: {e}'}), 400
    return jsonify({"message":'OK'}), 200



if __name__ == '__main__':

    # if we run this below, all devices in the same network (connected to the same router) 
    # can access the site via http://{local-network-IP}:5000/       
    # most times it is something like http://192.168.1.11:5000/
    # you can find your local network IP by typing ipconfig in cmd (windows) or ifconfig in terminal (linux)
    # or hostname -I in terminal (linux)
    # or in windows go to network settings and find your IP
    app.run(host='0.0.0.0', port=5000, use_reloader=False, debug=True)
    
    

