from http.server import HTTPServer, SimpleHTTPRequestHandler
import ssl
import os

os.chdir("artifacts")


class MyHTTPRequestHandler(SimpleHTTPRequestHandler):
    def end_headers(self):
        self.send_my_headers()

        SimpleHTTPRequestHandler.end_headers(self)

    def send_my_headers(self):
        self.send_header("Cross-Origin-Opener-Policy", "same-origin")
        self.send_header("Cross-Origin-Embedder-Policy", "require-corp")

httpd = HTTPServer(('localhost', 4443), MyHTTPRequestHandler)

httpd.socket = ssl.wrap_socket(httpd.socket,
                               keyfile="../key.pem",
                               certfile='../cert.pem', server_side=True)

httpd.serve_forever()