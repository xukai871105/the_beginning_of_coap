# -*- coding: utf-8 -*-
from flask import Flask, jsonify, render_template, request
app = Flask(__name__)

@app.route("/")
def index():
# 主页面
    return render_template('index.html')
    
@app.route('/value', methods=['GET', 'POST'])
def cal_value():
  if request.method == 'GET':
    a = request.args.get('a', 0, type=int)
    b = request.args.get('b', 0, type=int)
    return jsonify(result = a + b)
  elif request.method == 'POST':
    a = request.form.get('a', 0, type=int)
    b = request.form.get('b', 0, type=int)
    return jsonify(result = a + b)
    
if __name__=="__main__":
    app.run(host = '0.0.0.0',port = 8080, debug = True)
