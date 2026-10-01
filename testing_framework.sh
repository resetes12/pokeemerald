rm -rf *.gba
make tidymodern
./build-modern.sh
cp pokeemerald_modern.gba pokeemerald_modern_client1.gba
cp pokeemerald_modern.gba pokeemerald_modern_client2.gba
cp pokeemerald_modern.gba pokeemerald_modern_client3.gba
cp soullink.lua pokeemerald_modern.map /tmp/soullink-local/host
cp soullink.lua pokeemerald_modern.map /tmp/soullink-local/client1
cp soullink.lua pokeemerald_modern.map /tmp/soullink-local/client2
cp soullink.lua pokeemerald_modern.map /tmp/soullink-local/client3
