#ifndef COMMANDS_H
#define COMMANDS_H

enum AnimState{
    PLAY,
    PAUSE,
    BINDPOSE
};

enum VisualState{
    BONES,
    HIDDEN,
    VISIBLE
};

enum AnimCommands {
    PLAY_SOURCE,
    PLAY_DEST,
    BIND_SOURCE,
    BIND_DEST,
    HIDE_SOURCE,
    HIDE_DEST
};

enum LoadCommands{
    SOURCE,
    DEST
};

#endif // COMMANDS_H
