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
        }
    }
    // Event ClientSpawn
    if (Q_stricmp(Cmd_Argv(0), "ClientSpawn:") == 0) {

        if (atoi(Cmd_Argv(1)) < 100) {

            client_t *cl = PB_SearchUser(atoi(Cmd_Argv(1)));
            PB_CheckDeadorAlive( cl, cl, "spawn" );

        }
    }
    //Event ClientBegin
    if (Q_stricmp(Cmd_Argv(0), "ClientBegin:") == 0) {

        if (atoi(Cmd_Argv(1)) < 100) {

            client_t *cl = PB_SearchUser(atoi(Cmd_Argv(1)));
            // Bots armband et name color ( pas vraiment utile sauf pour la commande rcon players - résout le probléme [connecting] bot
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