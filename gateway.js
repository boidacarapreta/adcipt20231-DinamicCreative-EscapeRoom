const { SerialPort } = require('serialport')
const mqtt = require('mqtt')

const broker = 'wss://ifsc.digital/ws/'
const topic = 'adcipt20231/escape-room'
const client = mqtt.connect(broker)
  .on('error', error => console.log(error))
  .on('connect', () => console.log('Connected to %s.', broker))
  .on('packetsend', packet => console.log('Packet sent: %s: ', packet))

const prefix = '/dev/tty'
const ports = ['ACM', 'USB']
const numbers = [0, 1, 2]
const speed = 9600
ports.forEach(port => {
  numbers.forEach(number => {
    new SerialPort({
      path: prefix + port + number,
      baudRate: speed
    }).on('error', error => console.log(error))
      .on('open', serial => console.log(serial))
      .on('data', data => client.publish(topic, data, { qos: 2 }))
  })
})
