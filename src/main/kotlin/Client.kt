import java.io.PrintWriter
import java.net.Socket
import java.time.LocalDateTime
import java.time.format.DateTimeFormatter

class Client {
    private val totalPackets = 50000

    fun startClient() {
        val serverAddress = "192.168.1.11" // Remote Logstash server IP
        val serverPort = 8080 // Remote Logstash server port

        try {
            val socket = Socket(serverAddress, serverPort)
            println("Connected to remote Logstash server.")

            val writer = PrintWriter(socket.getOutputStream(), true)

            // Send packets to the Logstash server
            repeat(totalPackets) { packetNumber ->
                val message = "Packet $packetNumber"
                writer.println(message)
                println("Sent: $message")
            }

            // Print current timestamp after sending all packets
            val timestamp = LocalDateTime.now().format(DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss"))
            println("Current timestamp: $timestamp")

            // Close the socket
            socket.close()
        } catch (e: Exception) {
            println("Error: ${e.message}")
        }
    }
}

fun main() {
    val client = Client()
    client.startClient()
}
