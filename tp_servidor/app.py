from flask import Flask, request, jsonify
import sqlite3

app = Flask(__name__)
ARCHIVO_DB = "sensores.db"
@app.route('/')
def hola():
    return '<p>Hola!</p>'

def make_dicts(cursor, row):
    return dict((cursor.description[idx][0], value)
                for idx, value in enumerate(row))

@app.route('/api/sensor', methods=['POST'])
def recibir_valores():
    datos = request.json
    nombres = datos['nombres']
    potenciometro = datos['potenciometro']
    temperatura = datos['temperatura']
    humedad = datos['humedad']

    print(f'Mensaje recibido de {request.remote_addr}')
    print(f'Enviado por {nombres}')
    print(f'valor del potenciometro: {potenciometro}')
    print(f'valor de la temp: {temperatura}')
    print(f'valor de la humedad: {humedad}')

    db = sqlite3.connect(ARCHIVO_DB)
    db.row_factory = make_dicts
    try:
        db.execute("""INSERT INTO lecturas(nombre_sensor, valor_sensor)
                   VALUES(?, ?) """, ("potenciometro", potenciometro))
        db.execute("""INSERT INTO lecturas(nombre_sensor, valor_sensor)
                   VALUES(?, ?) """, ("temperatura", temperatura))
        db.execute("""INSERT INTO lecturas(nombre_sensor, valor_sensor)
                   VALUES(?, ?) """, ("humedad", humedad))
        db.commit()
    finally: db.close()
    return "OK", 200

@app.route("/api/lecturas")
def lecturas():
    db = sqlite3.connect(ARCHIVO_DB)
    db.row_factory = make_dicts
    try:
        cursor = db.execute("""SELECT id,
            nombre_sensor, valor_sensor,
            datetime(fecha_hora, '-3 hour')
            AS fecha_hora
            FROM lecturas;""")
        resultado = cursor.fetchall()
    finally: db.close()
    return jsonify(resultado)




    