#ifndef JUMP_H
#define JUMP_H

class Jump
{
    private:
        short _from, _to;
    public:
        Jump(short from, short to);
        short computeLanding();
};

#endif
