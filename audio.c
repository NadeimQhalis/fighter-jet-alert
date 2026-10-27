#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <mmsystem.h>

// Link the Windows Multimedia library to handle audio playback
#pragma comment(lib, "winmm.lib")

void draw_radar_screen(double distance, int is_collision_near) {
    system("cls"); // Refresh screen frame
    printf("==================================================\n");
    printf("        FIGHTER JET AVIONICS BUS (WAV PLAYER)     \n");
    printf("==================================================\n");
    
    if (distance > 3000.0) {
        printf("       [ . . . . . * . . . . . ]  (Target Out)\n");
    } else if (distance > 1500.0) {
        printf("       [ . . . . * . * . . . . ]  (Approaching)\n");
    } else {
        printf("       [ . . . . * . . . . ]  (CRITICAL CRASH)\n");
    }
    
    printf("--------------------------------------------------\n");
    printf("SENSOR RADAR DATA:\n");
    printf(" >> Proximity Distance: %7.1f meters\n", distance);
    
    if (is_collision_near) {
        printf(" >> COCKPIT ALERT Status: !!! PULL UP (PLAYING WAV) !!!\n");
    } else {
        printf(" >> COCKPIT ALERT Status: SCANNING / SECURE\n");
    }
    printf("==================================================\n");
}

int main() {
    double target_distance = 4500.0; 
    double closure_speed = 350.0;    
    double warning_threshold = 1500.0; 
    
    // 1. Automatically calculate the absolute path where the .exe is running
    char full_audio_path[MAX_PATH];
    GetModuleFileNameA(NULL, full_audio_path, MAX_PATH);
    
    // Strip off the executable filename to get just the folder path
    char *last_slash = strrchr(full_audio_path, '\\');
    if (last_slash != NULL) {
        *(last_slash + 1) = '\0'; // Cut string at the final backslash
    }
    
    // Append the exact target file name to the folder path
    strcat(full_audio_path, "jet_collision_alarm.wav");

    // 2. Explicitly verify the WAV file can be opened at this path
    FILE *file_check = fopen(full_audio_path, "rb");
    if (!file_check) {
        system("cls");
        printf("==================================================\n");
        printf("           DIAGNOSTIC CRITICAL FAILURE             \n");
        printf("==================================================\n");
        printf("ERROR: Could not open the audio payload.\n");
        printf("Searched path:\n -> %s\n\n", full_audio_path);
        printf("FIX: Please copy your 'jet_collision_alarm.wav' file\n");
        printf("and paste it into that exact folder listed above!\n");
        printf("==================================================\n");
        return 1;
    }
    fclose(file_check);

    printf("Booting path-verified avionics link...\n");
    Sleep(1000);

    while (target_distance > 0.0) {
        if (target_distance <= warning_threshold) {
            draw_radar_screen(target_distance, 1);
            
            // Play using the bulletproof absolute path string
            // Added SND_NODEFAULT to throw a clear error if the file format drops
            PlaySound((LPCSTR)full_audio_path, NULL, SND_FILENAME | SND_ASYNC | SND_NODEFAULT);
            
            printf(">> Executing evasive pitch maneuvers...\n");
            Sleep(3500); // Give the track time to complete audio streaming
            break; 
        } else {
            draw_radar_screen(target_distance, 0);
            Sleep(500); 
        }
        
        target_distance -= closure_speed;
    }
    
    PlaySound(NULL, 0, 0);
    printf("\n[Radar track terminated]\n");
    return 0;
}
