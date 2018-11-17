/*
===============================================================================================================================================================
PB Fonctions
Author      : PtitBigorneau
===============================================================================================================================================================
*/

#include "server.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
/*
===============================================================================
*/
///////////////////////////////////////////////////////////
//PB_SearchColorTeam
//////////////////////////////////////////////////////////
char *PB_SearchColorTeam(int n) {

    if (n == 0) {
        return "^2";
    }
    else if (n == 1) {
        return "^1";
    }
    else if (n == 2) {
        return "^4";
    }
    else if (n == 3) {
        return "^3";
    }
    else {return NULL;}
}
//////////////////////////
// PB_SearchWeapon
//////////////////////////
char *PB_SearchWeapon(int id) {

    if ( id == 12 ) {
        return "Knife";
    }
    else if ( id == 13 ) {
        return "Knife Thrown";
    }
    else if ( id == 14 ) {
        return "Beretta 92G";
    }
    else if ( id == 15 ) {
        return ".50 Desert Eagle";
    }
    else if ( id == 16 ) {
        return "Franchi Spas-12";
    }
    else if ( id == 17 ) {
        return "H&K Ump45";
    }
    else if ( id == 18 ) {
        return "H&K Mp5k";
    }
    else if ( id == 19 ) {
        return "ZM LR300 ML";
    }
    else if ( id == 20 ) {
        return "H&K G36";
    }
    else if ( id == 21 ) {
        return "H&K PSG-1";
    }
    else if ( id == 22 ) {
        return "H&K 69";
    }
    else if ( id == 23 ) {
        return "Bled";
    }
    else if ( id == 24 ) {
        return "shot of Boots";
    }
    else if ( id == 25 ) {
        return "HE Grenade";
    }
    else if ( id == 28 ) {
        return "Remington SR8";
    }
    else if ( id == 30 ) {
        return "Kalashnikov AK103";
    }
    else if ( id == 31 ) {
        return "Sploded";
    }
    else if ( id == 32 ) {
        return "Slapped";
    }
    else if ( id == 33 ) {
        return "Smited";
    }
    else if ( id == 34 ) {
        return "Bombed";
    }
    else if ( id == 35 ) {
        return "Nuked";
    }
    else if ( id == 36 ) {
        return "IMI Negev";
    }
    else if ( id == 37 ) {
        return "H&K 69";
    }
    else if ( id == 38 ) {
        return "M4A1";
    }
    else if ( id == 39 ) {
        return "Glock 18";
    }
    else if ( id == 40 ) {
        return "Colt 1911";
    }
    else if ( id == 41 ) {
        return "Ingram MAC-11";
    }
    else if ( id == 42 ) {
        return "FR-F1";
    }
    else if ( id == 43 ) {
        return "Benelli M4 Super 90";
    }
    else if ( id == 44 ) {
        return "FN Hesrstal P90";
    }
    else if ( id == 45 ) {
        return ".44 Magnum Revolver";
    }
    else if ( id == 46 ) {
        return "TOD-50";
    }
    else if ( id == 47 ) {
        return "Flag";
    }
    else if ( id == 48 ) {
        return "Goomba";
    }
    else {return "Unknown Weapon";}

}
//////////////////////////
// PB_SearchHitLocation
//////////////////////////
char *PB_SearchHitLocation(int id) {

    if ( id == 1 ) {
        return "Head";
    }
    else if ( id == 2 ) {
        return "Helmet";
    }
    else if ( id == 3 ) {
        return "Torso";
    }
    else if ( id == 4 ) {
        return "Vest";
    }
    else if ( id == 5 ) {
        return "Left Arm";
    }
    else if ( id == 6 ) {
        return "Right Arm";
    }
    else if ( id == 7 ) {
        return "Groin";
    }
    else if ( id == 8 ) {
        return "Butt";
    }
    else if ( id == 9 ) {
        return "Left Upper Leg";
    }
    else if ( id == 10 ) {
        return "Right Upper Leg";
    }
    else if ( id == 11 ) {
        return "Left Lower Leg";
    }
    else if ( id == 12 ) {
        return "Right Lower Leg";
    }
    else if ( id == 13 ) {
        return "Left Foot";
    }
    else if ( id == 14 ) {
        return "Right Foot";
    }
    else {return "Unknown";}

}
///////////////////////////////////////////////////////////
//PB_Replace_str
//////////////////////////////////////////////////////////
static char *PB_Replace_str(const char *s, const char *str1, const char *str2) {

    char *result;
    int i, cnt = 0;
    int nstr2 = strlen(str2);
    int nstr1 = strlen(str1);

    for (i = 0; s[i] != '\0'; i++) {
        if (strstr(&s[i], str1) == &s[i]) {
            cnt++;
             i += nstr1 - 1;
        }
    }

    result = (char *)malloc(i + cnt * (nstr2 - nstr1) + 1);
    i = 0;

    while (*s){
        if (strstr(s, str1) == s) {
            strcpy(&result[i], str2);
            i += nstr2;
            s += nstr1;
        }
        else {
            result[i++] = *s++;
        }
    }

    result[i] = '\0';
    return result;
}
//////////////////////////
//PB_SearchUser
//////////////////////////
client_t *PB_SearchUser(int cid) {

client_t *cl;
int j;

for (j = 0, cl = svs.clients; j < sv_maxclients->integer; j++, cl++) {
        if (!cl->state) { continue; }
        if (cid == j) { return cl; break;}
    }
    return NULL;
}
//////////////////////////
// PB_TestMapcycle
//////////////////////////
static int PB_TestMapcycle(const char *mapcycle){
    FILE* fichiern = NULL;
    fichiern = fopen(mapcycle, "r");

    if (fichiern != NULL) {
        return 1;
        fclose(fichiern);

    }
    else {return 0;}

}
//////////////////////////
// PB_Searchnextmap
//////////////////////////
static char *PB_Searchnextmap(void){

        char *gnextmapname;
        int testmc;
        gnextmapname = Cvar_VariableString( "g_nextmap" );

        if ((!Q_stricmp(gnextmapname, "") == 0) && (SV_GetMapSoundingLike(gnextmapname) !=NULL)){
            return SV_GetMapSoundingLike(gnextmapname);
        }
        else{

            FILE* mapsfile = NULL;
            char lmaps[128];

            char *mhomePath;
            char *mbasePath;
            char *mq3ut4Path;
            char *mapcyclefile;
            char *mapcyclefile2;
            char *filemapcycle = Cvar_VariableString( "g_mapcycle" );

            char *testb = NULL;
            int n = 0;
            char static firstmap[128];
            char *test = NULL;
            char *test2 = NULL;

            mhomePath = Cvar_VariableString( "fs_homePath" );

            mbasePath = Cvar_VariableString( "fs_basepath" );

            mq3ut4Path = Cvar_VariableString( "fs_game" );

            mapcyclefile = FS_BuildOSPath( mhomePath, mq3ut4Path , filemapcycle);
            mapcyclefile2 = FS_BuildOSPath( mbasePath, "q3ut4" , filemapcycle);

            testmc = PB_TestMapcycle(mapcyclefile);

            if (testmc == 1){mapsfile = fopen(mapcyclefile, "r");}
            else {mapsfile = fopen(mapcyclefile2, "r");}

            if (mapsfile != NULL) {
                while (fgets(lmaps, sizeof(lmaps), mapsfile) != NULL) {
                    Cmd_TokenizeString( lmaps );

                    if ((n == 0)&&(strcmp(Cmd_Argv(0),"") != 0)){
                        n++;
                        strcpy(firstmap,Cmd_Argv(0));

                    }

                    if (Q_stricmp(Cmd_Argv(0), sv_mapname->string)==0){

                        test = "ok";
                        continue;
                    }
                    if (test != NULL){

                        if (testb == NULL){

                            if (strpbrk(Cmd_Argv(0), "{") != NULL){
                                testb = "ok";
                                continue;}

                            if (strcmp(Cmd_Argv(0), "") == 0){continue;}
                            else{

                                return Cmd_Argv(0);
                                test2 = "ok";
                                break;
                            }

                        }

                        if (testb != NULL){
                            if (strpbrk(Cmd_Argv(0), "}") != NULL){testb = NULL;
                                continue;}
                        }
                    }
                }

                fclose(mapsfile);

            }

            if ((test == NULL)||(test2 == NULL)||(strcmp(Cmd_Argv(0), "") == 0)||strlen(Cmd_Argv(0))< 3){

                if (strlen(firstmap) < 3){
                    return Cvar_VariableString( "g_nextCycleMap" );
                }
                else {
                     return firstmap;
                }

            }

        }
    return NULL;
}
///////////////////////////////////////////////////////////
//PB_SearchIDWeapon
//////////////////////////////////////////////////////////
int PB_SearchIDWeapon(int powerups, int option) {

    int chargeurs = 0;
    int ammo = 0;
    int idweap = 0;

    if (powerups > 16777216) {
        chargeurs = powerups/16777216;
        ammo = (powerups -(chargeurs*16777216))/256;
        if (ammo >= 256) {
            powerups = powerups - 65536;
            chargeurs = powerups/16777216;
            ammo = (powerups -(chargeurs*16777216))/256;
               if (ammo >= 256) {
                powerups = powerups - 65536;
                chargeurs = powerups/16777216;
                ammo = (powerups -(chargeurs*16777216))/256;
            }
        }

        idweap = powerups - ((chargeurs*16777216)+(ammo*256));
    }
    else {
        ammo = powerups/256;
        if (ammo >= 256) {
            powerups = powerups - 65536;
            ammo = powerups/256;
        }
        idweap = powerups - (ammo*256);
        chargeurs = 0;
    }

    if (option == 1) { return idweap; }
    if (option == 2) { return chargeurs; }
    if (option == 3) { return ammo; }

    return 0;

}
/*
===============================================================================================================================================================
PB_ControlWeapons
===============================================================================================================================================================
*/
/*
=======================
PB_ControlStamina
=======================
*/
void PB_ControlStamina( client_t *cl ) {

    playerState_t  *ps = SV_GameClientNum( cl - svs.clients );
    int weap = PB_SearchIDWeapon(ps->powerups[ps->weapon], 1);
    if (weap == 11 || weap == 12 || weap == 16 || weap < 2 ) {ps->stats[STAT_STAMINA] = ps->stats[STAT_HEALTH] * 300;}

}

/*
=======================
PB_ControlWeapons
=======================
*/
void PB_ControlWeapons( client_t *cl ) {

    if (pb_knifefullstamina->integer == 1) {
        PB_ControlStamina( cl );
    }
 }
/*
===============================================================================================================================================================
PB GameControl
===============================================================================================================================================================
*/
/*
=======================
PB_Pub
=======================
*/
void PB_Pub( void ) {

    if (!Q_stricmp(pb_filepub->string, "")) {
        return;
    }

    if (pb_timepub->integer !=0 || pb_timepub->integer > 5 || pb_timepub->integer < 3601) {

        static int lastTime = 0;
        static int lastLigne = 0;

        char *positionpub;

        if (!Q_stricmp(pb_positionpub->string, "chat")) {positionpub = "chat";}
        else if (!Q_stricmp(pb_positionpub->string, "cp")) {positionpub = "cp";}
        else {positionpub = "print";}

        char *pubhomePath;
        char *pubq3ut4Path;

        pubhomePath = Cvar_VariableString( "fs_homePath" );

        pubq3ut4Path = Cvar_VariableString( "fs_game" );

        if (lastTime + pb_timepub->integer*1000 <= svs.time){

            FILE* filepub = NULL;
            static char ligne[1024]="";
            filepub = fopen(FS_BuildOSPath( pubhomePath, pubq3ut4Path, pb_filepub->string), "r");

            if (!filepub) { return; }

            int NombreLignes = 0;
            int i;
            if (filepub != NULL) {

                i = 0;

                while (fgets(ligne, sizeof(ligne), filepub) != NULL) {

                    char a[64] = "";
                    char b[64] = "";
                    char *nligne = NULL;

                    if (strcmp(ligne, "\n") != 0){

                        if(strstr(ligne, "@map") != NULL) {

                            Com_sprintf(a, sizeof(a), "@map");
                            Com_sprintf(b, sizeof(b), "%s", sv_mapname->string);
                        }

                        if(strstr(ligne, "@time") != NULL) {

                            Com_sprintf(a, sizeof(a), "@time");

                            time_t timestamp;
                            struct tm * t;
                            timestamp = time(NULL);
                            t = localtime(&timestamp);

                            Com_sprintf(b, sizeof(b), "%02u:%02u", t->tm_hour, t->tm_min);
                        }

                        if(strstr(ligne, "@nextmap") != NULL) {
                            char *nextmap = PB_Searchnextmap();
                            Com_sprintf(a, sizeof(a), "@nextmap");
                            Com_sprintf(b, sizeof(b), "%s", nextmap);
                        }

                        if (Q_stricmp(a, "")) {

                            nligne = ligne;
                            nligne = PB_Replace_str(ligne, a, b);
                            Com_sprintf(ligne, sizeof(ligne), nligne);
                        }

                        if (lastLigne == i) {
                            SV_SendServerCommand(NULL, "%s \"%s\"", positionpub, ligne);

                        }
                        i++;
                    }
                    NombreLignes++;
                }

            }

            lastLigne++;

            if ( lastLigne >= NombreLignes) {lastLigne = 0;}
            lastTime = svs.time;
            fclose(filepub);
        }

    }
}
/*
=======================
PB_GameControl
=======================
*/
void PB_GameControl( void ) {

    if (Q_stricmp(pb_filepub->string, "") != 0 && pb_timepub->integer !=0) {
        if (sv.time > 5000){
            PB_Pub();
        }
    }

}
/*
===============================================================================================================================================================
PB Events
===============================================================================================================================================================
*/
/*
=======================
PB_CheckDeadorAlive
=======================
*/
static void PB_CheckDeadorAlive( client_t *aclient, client_t *vclient, char *arg ) {

    playerState_t  *ps;

    int            t, min, tens, sec;

    sec = ( sv.time - sv.restartTime ) / 1000;
    t = ( sv.time - sv.restartTime );
    min = sec / 60;
    sec -= min * 60;
    tens = sec / 10;
    sec -= tens * 10;

    char *homePath;
    char *q3ut4Path;
    char *logpath;

    int aclid = aclient - svs.clients;
    int vclid = vclient - svs.clients;

    homePath = Cvar_VariableString( "fs_homePath" );

    q3ut4Path = Cvar_VariableString( "fs_game" );

    logpath = FS_BuildOSPath( homePath, q3ut4Path, Cvar_VariableString("g_log"));

    ps = SV_GameClientNum( vclient - svs.clients );

    FILE* fichier = NULL;

    fichier = fopen(logpath, "a");

    if (fichier != NULL)
    {
        if (strcmp(arg,"spawn") == 0){
            fprintf(fichier, "%3i:%i%i ClientAlive: %i %f %f %i\n", min, tens, sec, vclid, ps->origin[0], ps->origin[1], t);

        }
        if (strcmp(arg,"dead") == 0){
            fprintf(fichier, "%3i:%i%i ClientDead: %i %i %f %f %i\n", min, tens, sec, vclid, aclid, ps->origin[0], ps->origin[1], t);

        }
        fclose(fichier);
    }

}
/*
=======================
PB_KillDistance
=======================
*/
static void PB_KillDistance( client_t *aclient, client_t *vclient, int idweapon ) {

    if (aclient - svs.clients == vclient - svs.clients)
    {
        return;
    }

    playerState_t  *aps = SV_GameClientNum( aclient - svs.clients );
    playerState_t  *vps = SV_GameClientNum( vclient - svs.clients );

    float dx = aps->origin[0] - vps->origin[0];
    float dy = aps->origin[1] - vps->origin[1];
    float adx = (float)fabs(dx);
    float ady = (float)fabs(dy);
    float dx2dy2 = (adx * adx) + (ady * ady);

    float dist = (float)sqrtf(dx2dy2);

    char *weapon = PB_SearchWeapon(idweapon);

    int ateam = aps->persistant[PERS_TEAM];
    int vteam = vps->persistant[PERS_TEAM];

    char acname[64];
    char vcname[64];

    int hitid = vclient->lasthitlocation;

    char *hitlocation = PB_SearchHitLocation(hitid);

    Q_strncpyz(acname, aclient->name, sizeof(acname));
    Q_CleanStr(acname);

    Q_strncpyz(vcname, vclient->name, sizeof(vcname));
    Q_CleanStr(vcname);

    if (idweapon != 22 && idweapon != 23 && idweapon != 25 && idweapon != 37) {

        if (( sv_gametype->integer > 2 && sv_gametype->integer < 9 ) || sv_gametype->integer == 11 ) {

            if (ateam == vteam ) {
                SV_SendServerCommand(NULL, "print\"%s%s ^7killed %s%s^7 at ^3%.2f m ^7[%s][%s]\"\n", PB_SearchColorTeam(ateam), acname, PB_SearchColorTeam(vteam), vcname, ((dist/100)+((dist/100*0.625))), hitlocation, weapon );
                return;
            }
        }

        SV_SendServerCommand(NULL, "print\"%s%s ^7killed %s%s^7 at ^3%.2f m ^7[%s][%s]\"\n", PB_SearchColorTeam(ateam), acname, PB_SearchColorTeam(vteam), vcname, ((dist/100)+((dist/100*0.625))), hitlocation, weapon );

    }
    else {

        SV_SendServerCommand(NULL, "print\"%s%s ^7killed %s%s^7 at ^3%.2f m ^7[%s]\"\n", PB_SearchColorTeam(ateam), acname, PB_SearchColorTeam(vteam), vcname, ((dist/100)+((dist/100*0.625))), weapon );
    }

}
/*
=======================
PB_Events
=======================
*/
void PB_Events(char event[1024])
{
    if (sv_gametype->integer == 9){ return; }

    Cmd_TokenizeString( event );

    // Event Kill
    if (Q_stricmp(Cmd_Argv(0), "Kill:") == 0) {
        if (atoi(Cmd_Argv(1)) == 1022 && atoi(Cmd_Argv(2)) < 100) {

            client_t *vcl = PB_SearchUser(atoi(Cmd_Argv(2)));
            client_t *acl = vcl;
            PB_CheckDeadorAlive( acl, vcl, "dead" );

        }
        if (atoi(Cmd_Argv(1)) < 100 && atoi(Cmd_Argv(2)) < 100) {

            client_t *acl = PB_SearchUser(atoi(Cmd_Argv(1)));
            client_t *vcl = PB_SearchUser(atoi(Cmd_Argv(2)));

            PB_CheckDeadorAlive( acl, vcl, "dead" );

            if (Cvar_VariableValue("g_loghits") != 0) {
                PB_KillDistance( acl, vcl, atoi(Cmd_Argv(3)));
            }

        }
    }
    // Event ClientSpawn
    if (Q_stricmp(Cmd_Argv(0), "ClientSpawn:") == 0) {

        if (atoi(Cmd_Argv(1)) < 100) {

            client_t *cl = PB_SearchUser(atoi(Cmd_Argv(1)));
            playerState_t  *ps = SV_GameClientNum( cl - svs.clients );

            PB_CheckDeadorAlive( cl, cl, "spawn" );

            if (Cvar_VariableValue("g_loghits") != 0) {
                if (ps->persistant[PERS_SPAWN_COUNT] < 2 || ps->persistant[PERS_HITS] == 0) {

                    cl->lasthitlocation = -1;

                }
            }
        }
    }
    // Event Hit
    if (Q_stricmp(Cmd_Argv(0), "Hit:") == 0) {

        if (atoi(Cmd_Argv(1)) < 100 && atoi(Cmd_Argv(2)) < 100) {
            if (Cvar_VariableValue("g_loghits") != 0) {
                client_t *vcl = PB_SearchUser(atoi(Cmd_Argv(1)));
                 vcl->lasthitlocation = atoi(Cmd_Argv(3));
            }

        }
    }
    //Event ClientBegin
    if (Q_stricmp(Cmd_Argv(0), "ClientBegin:") == 0) {

        if (atoi(Cmd_Argv(1)) < 100) {

            client_t *cl = PB_SearchUser(atoi(Cmd_Argv(1)));
            // Bots armband et name color ( pas vraiment utile sauf pour la commande rcon players - résout le probléme [connecting] bot )
            if ( cl->netchan.remoteAddress.type == NA_BOT ) {
                char *armband = "0,255,255";
                char *color ="^5";
                char name[64];
                strcpy(name, color);
                strcat (name, cl->name);
                Info_SetValueForKey(cl->userinfo, "name", name);
                Info_SetValueForKey(cl->userinfo, "cg_RGB", armband);
                SV_UserinfoChanged(cl);
                VM_Call(gvm, GAME_CLIENT_USERINFO_CHANGED, cl - svs.clients);

            }
        }
    }
}