#ifndef _PLAYER_FUNC_H
#define _PLAYER_FUNC_H

void play_song(void);
void pause_song(void);
void song_forback_ward(int32_t sec);
void equalizer_tuning(uint8_t band1, uint8_t band2, uint8_t band3, uint8_t band4, uint8_t band5);
void player_display_cover(char* file_name);

#endif
