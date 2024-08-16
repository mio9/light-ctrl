<template>
  <div>
    <div>
      <input type="text" placeholder="Type hex..." v-model="hexInput" />
      <button @mousedown.prevent="lightOn" @mouseup.prevent="lightOff">Light</button>
    </div>

    <div>
      <input type="text" v-model="wsUrl">
      <button @click="connect">Connect</button>
    </div>

    <p>WS Connected: {{ wsConnected }}</p>
    <p>{{ wsError }}</p>
  </div>
</template>
<script setup lang="ts">
import { hexToBytes, bytesToBase64 } from "./lib/hex"
const wsConnected = ref(false)
const wsError = ref("")
const hexInput = ref("")
const wsUrl = ref("ws://localhost:1880/ws")

let connection: WebSocket | null = null;

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
  sendData(hexInput.value);
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