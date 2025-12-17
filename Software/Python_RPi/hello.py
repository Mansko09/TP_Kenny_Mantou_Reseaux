from flask import Flask,jsonify, abort, render_template, request
import json
app = Flask(__name__)

@app.route('/')
def hello_world():
        return 'Hello, world!\n'

welcome=list("Welcome to 3ESE API!")

@app.route('/api/request/', methods=['GET','POST'])
@app.route('/api/request/<path>', methods=['GET','POST'])
def api_request(path=None):
    resp = {
            "method":   request.method,
            "url" :  request.url,
            "path" : path,
            "args": request.args,
            "headers": dict(request.headers),
    }
    if request.method == 'POST':
        resp["POST"] = {
                "data" : request.get_json(),
                }
    return jsonify(resp)

@app.errorhandler(404)
def page_not_found(error):
        return render_template('page_not_found.html'),404


@app.route('/api/welcome/<int:index>', methods=['GET', 'PUT', 'PATCH', 'DELETE'])
def api_welcome_index(index):
    global welcome

    # Index invalide (sauf PUT insertion)
    if index >= len(welcome) and request.method != 'PUT':
        abort(404)

    # READ
    if request.method == 'GET':
        return jsonify({
            "index": index,
            "val": welcome[index]
        })

    # PUT : insertion à l’index
    elif request.method == 'PUT':
        data = request.get_json()
        if not data or 'val' not in data:
            abort(400)

        word = data['val']
        welcome.insert(index, word)

        return jsonify(welcome)

    # PATCH : modification d’un caractère
    elif request.method == 'PATCH':
        data = request.get_json()
        if not data or 'val' not in data:
            abort(400)

        char = data['val']
        if len(char) != 1:
            return jsonify({"error": "Erreur longueur"}), 400

        welcome[index] = char
        return jsonify(welcome)

    # DELETE
    elif request.method == 'DELETE':
        welcome.pop(index)
        return "", 204

@app.route('/api/welcome/', methods=['GET', 'POST', 'DELETE'], strict_slashes=False)
def api_welcome_global():
    global welcome

    if request.method == 'GET':
        return jsonify(welcome)

    elif request.method == 'POST':
        data = request.get_json()
        if not data or 'val' not in data:
            abort(400)

        welcome.append(data['val'])
        return "", 202

    elif request.method == 'DELETE':
        welcome.clear()
        return "", 204
