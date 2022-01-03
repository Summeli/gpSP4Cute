gpSP4SCute: [gpsp][gpsp] for Symbian
===========================

gpSP4Cute is a gameboy advanced emulator for Symbian. It's based on gpSP version 0.9-2Xb by Exophase for gp2x.


Building
----------
The gpSP stuff is build separately with custom makefilesystem, and the gpsp4cute is build with the usual Qt flags.

The gpSP lib should be build with [General Makefile system by summeli] (https://summeli.com/2247)
You'll also will get some errors from the checklib when buildin the gpsp. Therefore you should [get rid of the checklib 
in QtSDK] (https://www.summeli.com/2492)

After the gpSP lib is build, you can build the gpsp4cute project which will add the UI to the gpsp project. The gpsp4cute
is build with standard Qt-stuff. See my blog post how to set up the environement with QtSDK [www.summeli.com] (https://summeli.com/2277)

License
-------

gpsp is distributed under [GPLv2 license](https://github.com/Summeli/gpSP4Symbian/blob/master/COPYING.DOC)

Credits
-------
* Exophase [gpsp] - Creator of original gpSP
* Zodttd [Zodttd] - Helping Exophase with ARM port
* Summeli [summeli] - Symbian port
* Olli Hinkka - Memory Relocator code
* Saiyaku - graphics and app icon

[gpsp]: http://gpsp-dev.blogspot.com/ "gpSP development bblog"
[summeli]: https://summeli.com
[Zodttd]: http://www.zodttd.com/
