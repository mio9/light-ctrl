<template>
  <div>
    <div>
      <input class="w-[300px] h-[200px]" type="color" placeholder="Type hex..." v-model="hexInput" />{{ hexInput }}
      
      <button class="btn btn-primary w-1/3" @mousedown.prevent="lightOn" @mouseup.prevent="lightOff">Flash</button>
      <p>brightness control</p>
      <input type="range" min="0" max="255" label="brightness" v-model="brightness" class="range range-primary" />
    </div>

    <div>
      <input class="input input-bordered" type="text" v-model="wsUrl">
      <button :class="`btn ${wsConnected? 'btn-success': 'btn-primary'}`" @click="connect">Connect</button>
      <button class="btn" @click="setLanURL">Set LAN URL</button>
    </div>

    

    <button class="btn btn-secondary w-1/3" @click="lightOn">On</button>
    <button class="btn w-1/3" @click="lightOff">Off</button>

    <p>WS Connected: {{ wsConnected }}</p>
    <p>{{ wsError }}</p>
  </div>
</template>
<script setup lang="ts">
import { hexToBytes, bytesToBase64 } from "./lib/hex"
import { rgbToHsl, hslToRgb } from "./lib/color";

const wsConnected = ref(false)
const wsError = ref("")
const hexInput = ref("#000000")
const brightness = ref(20)
const wsUrl = ref("ws://localhost:1880/ws")

let connection: WebSocket | null = null;

function setLanURL() {
  wsUrl.value = "ws://192.168.20.194/ws"
}

function connect() {
  wsError.value = ""
  connection = new WebSocket(wsUrl.value)

  connection.onopen = () => {
    console.log("Connected")
    wsConnected.value = true
  }

  connection.onerror = (error) => {
    console.log(error)
    wsError.value = "WS Errored"
  }

  connection.onclose = () => {
    console.log("Disconnected")
  }
}



function lightOn() {
  const color = hexInput.value.slice(1)
  // scale down the color to just 10% energy
  // const rgb = [color.substring(0, 2), color.substring(2, 4), color.substring(4, 6)]
  // console.log(rgb)
  const rgbBytes = hexToBytes(color)
  console.log(rgbBytes)
  // const hsv = rgbToHsl()

  // sendData(color);
}

function lightOff() {
  sendData("000000");
}

function sendData(value: string) {
  const bytes = hexToBytes(value) // hexToBytes(hexInput.value)
  const base64 = bytesToBase64(bytes)
  console.log(bytes)
  console.log(base64)

  connection.send(base64)
}
</script>