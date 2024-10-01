const express = require("express");
const http = require("http");
const { SerialPort } = require("serialport");
const { ReadlineParser } = require("@serialport/parser-readline");
const bodyParser = require("body-parser");
const cors = require("cors");
const app = express();
const server = http.createServer(app);
const jsonParser = bodyParser.json();

const port = new SerialPort(
  {
    path: "COM8",
    baudRate: 9600,
  },
  (err) => {
    if (err) {
      return console.log("Error on create:", err.message);
    }
  }
);

const parser = port.pipe(new ReadlineParser({ delimiter: "\r\n" }));

parser.on("error", (err) => {
  console.log("Error:", err.message);
});

parser.on("data", (data) => {
  console.log("Response:", data);
});

app.use(cors());

app.post("/change", jsonParser, (req, res) => {
  const { action, speed, delay } = req.body;

  let command = "";

  if (action === "start_motor") {
    command = "START_MOTOR\n";
  } else if (action === "stop_motor") {
    command = "STOP_MOTOR\n";
  } else if (action === "start_strobe") {
    command = "START_STROBE\n";
  } else if (action === "stop_strobe") {
    command = "STOP_STROBE\n";
  } else if (action === "strobe_delay") {
    command = `STROBE_DELAY ${delay}\n`;
  } else if (action === "motor_speed") {
    command = `MOTOR_SPEED ${speed}\n`;
  }

  port.write(command, (err) => {
    if (err) {
      console.error("Error writing to serial port:", err);
      res.status(500).json({ message: "Error writing to serial port." });
    } else {
      console.log(`Command sent: ${command.trim()}`);
      res.json({ message: "Success!" });
    }
  });
});

app.get("/", (req, res) => {
  res.sendFile(__dirname + "/stroboscope.html");
});

server.listen(3000, () => {
  console.log("Server running on port 3000. Visit http://localhost:3000 on your browser.");
});