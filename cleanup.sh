#!/bin/bash

echo "🧹 Iniciando limpeza do projeto..."

# Remover arquivos Arduino (.ino)
echo "🗑️ Removendo arquivos Arduino..."
rm -f *.ino
rm -f config.h

# Remover pasta Arduino
echo "🗑️ Removendo pasta sistema_final_integrado..."
rm -rf sistema_final_integrado/

# Remover arquivos de log
echo "🗑️ Removendo arquivos de log..."
rm -f server.log

# Remover pasta dist (pode ser recriada com npm run build)
echo "🗑️ Removendo pasta dist..."
rm -rf dist/

echo "✅ Limpeza concluída!"
echo ""
echo "📁 Arquivos essenciais mantidos:"
echo "   - package.json"
echo "   - src/"
echo "   - public/"
echo "   - index.html"
echo "   - node_modules/"
echo "   - arquivos de configuração (.ts, .json)"
echo ""
echo "🚀 Para fazer o build novamente: npm run build" 