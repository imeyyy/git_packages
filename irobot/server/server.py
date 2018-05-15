# library imports
from flask import Flask
from flask import render_template
from flask import request
from flask import jsonify

# our imports
from controller import Controller


########################################################################


# creates a Flask application, named app
app = Flask(__name__)
app.config['SEND_FILE_MAX_AGE_DEFAULT'] = 0


# create our cController instance
controller = Controller()


# a route where we will display a welcome message via an HTML template
@app.route("/", methods=['GET', 'POST'])
def hello():
    if request.method == 'GET':
        return render_template('index.html')
    if request.method == 'POST':
        command = request.form['command']
        args = request.form['args']
        res = controller.do(command=command, args=args)
        return jsonify(res)


# run the application
if __name__ == "__main__":
    app.run(debug=True, host='0.0.0.0', port=3000)
