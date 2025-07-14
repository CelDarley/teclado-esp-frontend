#!/usr/bin/env python3
"""
Servidor web simples para servir o frontend Vue.js
"""

import http.server
import socketserver
import os
import sys
from pathlib import Path

# Configuração
PORT = 8080
DIRECTORY = "."

class CustomHTTPRequestHandler(http.server.SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=DIRECTORY, **kwargs)
    
    def end_headers(self):
        # Adicionar headers CORS para desenvolvimento
        self.send_header('Access-Control-Allow-Origin', '*')
        self.send_header('Access-Control-Allow-Methods', 'GET, POST, PUT, DELETE, OPTIONS')
        self.send_header('Access-Control-Allow-Headers', 'Content-Type, Authorization')
        super().end_headers()
    
    def do_OPTIONS(self):
        self.send_response(200)
        self.end_headers()
    
    def translate_path(self, path):
        # Para SPA (Single Page Application), sempre servir index.html
        if not os.path.exists(os.path.join(DIRECTORY, path.lstrip('/'))):
            return os.path.join(DIRECTORY, 'index.html')
        return super().translate_path(path)

def main():
    # Verificar se o diretório dist existe
    if not os.path.exists(DIRECTORY):
        print(f"Erro: Diretório '{DIRECTORY}' não encontrado!")
        print("Execute 'npm run build' primeiro para gerar os arquivos.")
        sys.exit(1)
    
    # Mudar para o diretório do script
    script_dir = Path(__file__).parent
    os.chdir(script_dir)
    
    # Criar servidor
    with socketserver.TCPServer(("0.0.0.0", PORT), CustomHTTPRequestHandler) as httpd:
        print(f"🚀 Servidor rodando em http://0.0.0.0:{PORT}")
        print(f"📁 Servindo arquivos de: {os.path.abspath(DIRECTORY)}")
        print("⏹️  Pressione Ctrl+C para parar")
        
        try:
            httpd.serve_forever()
        except KeyboardInterrupt:
            print("\n👋 Servidor parado!")

if __name__ == "__main__":
    main() 