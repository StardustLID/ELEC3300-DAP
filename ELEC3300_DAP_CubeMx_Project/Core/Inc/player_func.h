#ifndef _PLAYER_FUNC_H
#define _PLAYER_FUNC_H

void play_song(void);
void pause_song(void);
void song_forback_ward(int32_t sec);
void equalizer_tuning(uint8_t band1, uint8_t band2, uint8_t band3, uint8_t band4, uint8_t band5);
void player_display_cover(char* file_name);
uint8_t write_song_to_play_list(uint8_t song, uint8_t tar_play_list);
uint8_t* read_playlist(uint8_t playlist_num, uint8_t* size);

#endif
