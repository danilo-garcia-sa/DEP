from flask import Flask, request, jsonify
import sqlite3

app = Flask(__name__)
ARCHIVO_DB = "Sensores.db"

@app.route('/')
def hola():
    return '<p>Hola!</p>'

def make_dicts(cursor, row):
    return dict((cursor.description[idx][0], value)
                for idx, value in enumerate(row))

@app.route('/api/sensor', methods=['POST'])
def recibir_valores():
    datos = request.json
    nombres     = datos.get('nombres')
    valor       = datos.get('potenciometro')
    temperatura = datos.get('temperatura')
    humedad     = datos.get('humedad')

    print(f'Mensaje recibido de: {request.remote_addr}')
    print(f'Enviado por: {nombres}')
    print(f'Valor del potenciómetro: {valor}')
    print(f'Temperatura: {temperatura} °C')
    print(f'Humedad: {humedad} %')

    return 'ok'