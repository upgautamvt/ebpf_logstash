import java.io.BufferedReader
import java.io.InputStreamReader
import java.net.ServerSocket
import java.net.Socket
import java.time.LocalDateTime
import java.time.format.DateTimeFormatter
import kotlin.system.exitProcess

class Server {
    private val totalPackets = 50000
    private val lastPacketIndex = totalPackets - 1

    fun startServer() {
        val serverSocket = ServerSocket(9090) // Listen on port 8080
        println("Server started. Waiting for Logstash messages...")

        val receivedMessages = mutableListOf<String>()
        var packetsReceived = 0

        while (packetsReceived < totalPackets) {
            val clientSocket = serverSocket.accept()
            println("Logstash connected: ${clientSocket.inetAddress.hostAddress}")

            val clientHandler = ClientHandler(clientSocket, receivedMessages)
            clientHandler.start()
            packetsReceived++
        }

        // Wait for all packets to be received
        while (receivedMessages.size < totalPackets) {
            Thread.sleep(100) // Sleep for a short duration to avoid busy-waiting
        }

        // Print received messages
        println("Received messages:")
        receivedMessages.forEach { println(it) }
    }

    inner class ClientHandler(private val clientSocket: Socket, @Suppress("UNUSED_PARAMETER") private val receivedMessages: MutableList<String>) : Thread() {
        private val reader = BufferedReader(InputStreamReader(clientSocket.getInputStream()))

        override fun run() {
            var message: String?
            while (true) {
                message = reader.readLine()
                if (message == null) break // Exit the loop if end of stream is reached
                println("Logstash says: $message")
                receivedMessages.add(message)

                if (receivedMessages.size == totalPackets && receivedMessages.joinToString(" ").contains(lastPacketIndex.toString())) {
                    // Print current timestamp
                    val timestamp = LocalDateTime.now().format(DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss"))
                    println("Current timestamp: $timestamp")
                    // Stop the server process
                    exitProcess(0)
                }
            }
        }
    }
}

fun main() {
    val server = Server()
    server.startServer()
}
