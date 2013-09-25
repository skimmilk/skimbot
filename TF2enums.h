#pragma once
enum playercond_t
{
    TFCOND_SLOWDOWN = (1<<0), // Heavy minigun, sniper huntsman, sniper rifle scoped
    TFCOND_SCOPED = (1<<1), // Only sniper rifle
    TFCOND_DISGUISING = (1<<2),
    TFCOND_DISGUISED = (1<<3),
    TFCOND_CLOAKED = (1<<4),
    TFCOND_INVULN = (1<<5),
    TFCOND_TELEPARTICLES = (1<<6),
    TFCOND_TAUNT = (1<<7),
    TFCOND_INVULNFADE = (1<<8),
    TFCOND_BUMPED = (1<<9), // Bumping into an invis spy
    TFCOND_TELEPORTING = (1<<10),
    TFCOND_CRITBOOST = (1<<11),

    TFCOND_PHASING = (1<<14),
    TFCOND_STUNNED = (1<<15), // For everything that affects your movement such as airblast, bonk slowdown, fan, etc. check stunflags for more info
    TFCOND_MINIBUF = (1<<16),
    TFCOND_CHARGING = (1<<17),

    TFCOND_HEALED = (1<<20),
    TFCOND_ONFIRE = (1<<21),
    TFCOND_OVERHEAL = (1<<22),
    TFCOND_JARATE = (1<<23),
};