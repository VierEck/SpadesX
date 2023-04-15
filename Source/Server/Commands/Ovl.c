#include <Server/Packets/Packets.h>
#include <Server/Player.h>
#include <Server/Server.h>
#include <Util/Checks/PlayerChecks.h>
#include <Util/Log.h>
#include <Util/Notice.h>

void cmd_ovl(void* p_server, command_args_t arguments)
{
    server_t* server = (server_t*) p_server;
    if (arguments.console) {
        LOG_INFO("You cannot use this command from console");
        return;
    }
    if (arguments.player->using_ovl == 1) {
        arguments.player->using_ovl = 0;

        arguments.player->movement.position.z += 2;
        send_create_player(server, arguments.player, arguments.player);
        arguments.player->movement.position.z -= 2;

        //todo: send kill packet only to this specific ovl user if he dead. 

        //todo: adjust orientation to what it was before entering ovl. 

        send_server_notice(arguments.player, arguments.console, "You are no longer a hidden spectator");
    } else if (arguments.player->using_ovl == 0) {
        arguments.player->using_ovl = 1;

        if (is_past_join_screen(arguments.player)) {
            team_t team = arguments.player->team;
            arguments.player->team = 2;
            send_create_player(server, arguments.player, arguments.player);
            arguments.player->team = team;

            //todo: spawn fake localplayer. (good for ESP traps)
        }
        send_server_notice(arguments.player, arguments.console, "You are now a hidden spectator");
    }
}