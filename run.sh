
./receiver \
| ffmpeg -re -i - -g 50 -movflags frag_keyframe+empty_moov+default_base_moof -f mp4 - \
| ./websocket