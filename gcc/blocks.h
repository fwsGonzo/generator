#ifndef BLOCKS_H
#define BLOCKS_H

#define max_unique_ids  1023

#define air_end           23

#define stone_start       24
#define stone_end         98

#define soil_start       100
#define soil_end         159

#define sand_start       160
#define sand_end         199

#define halfblock_start  400
#define halfblock_end    459

#define lowblock_start   460
#define lowblock_end     503

#define alpha_barrier    496

#define trees_start      504
#define trees_end        549

#define supertrans_start 580
#define supertrans_end   649

#define cross_start      750
#define cross_underwater 920
#define cross_end        959

#define blend_barrier    960

#define _AIR           0 // air is ignored
#define _FOG           1 // fog block

#define _ADMINIUM     24 // les indestructible

#define _STONEHEAP    25 // stones
#define _STONE        26
#define _REDOCHRE     27
#define _STONEMOSSYGREEN    30
#define _STONEMOSSYJUNGLE   31
#define _STONEMOSSYYELLOW   32
#define _STONEMOSSYDARK     33
#define _STONEMOSSYBROWN    34
#define _STONEMOSSYFLOWERS  35
#define _STONESNOWY         36

#define _COAL        40 // ores
#define _IRON        41
#define _GOLD        42
#define _REDSTONE    43
#define _GREENSTONE  44
#define _DIAMOND     45
#define _MAGICSTONE  46
#define _LIVINGSTONE 47

#define _ASPHALT     50

#define _IRONBLOCK   55
#define _GOLDBLOCK   56
#define _DIAMBLOCK   57
#define _ICECUBE     58

#define _MOLTENSTONE   62

#define _GREENSOIL     100 // earth
#define _GREENGRASS_S  101
#define _GREENGRASS    102
#define _SNOWSOIL      103
#define _SNOWGRASS_S   104
#define _SNOWGRASS     105

#define _SANDBEACH   160
#define _SANDOCEAN   161
#define _OCEANFLOOR  162 // ocean sand
#define _DESERTSAND  163 // desert sand

#define _GRAVEL1     167 // brown gravel
#define _GRAVEL2     168 // grey gravel

#define _CLAY        195 // blue-grey clay
#define _CLAYRED     196 // red-grey clay

#define _WOODBROWN  200 // ministry of forestry
#define _WOODBJORK  201
#define _WOODPALM   202
#define _WOODPINE   203
#define _PLANK      204
#define _WORKBENCH  205 // all-in-one workbench /- plankmaster-2000 -/
#define _CHEST      206
#define _BOOKCASE   207

#define _PUMPKIN     231
#define _CACTUSBLOCK 232

#define _GIANTSHROOMCORE      250
#define _GIANTSHROOMCORETOP   251
#define _GIANTSHROOMUNDERTOP  252
#define _GIANTSHROOMTOP       253
#define _GIANTSHROOMTOPEDGE   254
#define _GIANTSHROOMTOPSPECLE 255

#define _FURNACE        300  // player tool blocks
#define _FURNACEBURNING 301

#define _BRICKWALL      306
#define _BRICKWALL2     307

#define _SPONGE         330
#define _TRAMPOLINE     333
#define _TNT			334

#define _GRADIENT_SOFT  337 // soft paletted blocks
#define _GRADIENT_HARD  338 // hard paletted blocks


///  half blocks 400 -> 459 ///

#define _HALFSTONE      400
#define _HALFSTONETILE  401
#define _HALFSTONEPLATE 402
#define _HALFBRICK      403
#define _HALFBRICK2     404

#define _HALFWOOD       410


// special case: kill only bottom / sides matching other halfs
#define _STONESTAIR     450
#define _WOODSTAIR      451
#define _BRICKSTAIR     452

///  low blocks 460 -> 503 ///

#define _LOWSTONETILE   480
#define _LOWSTONETILE2  481
#define _LOWSNOW        482
#define _LOWICE         483 // ice

///  alpha blocks 504+ ///

///  leaf 504-549 ///
#define _LEAF_LEAFS    504 // more forestry

#define _LEAF_NEEDLE   510

#define _VINES         545

// --------------------------
#define _LADDER        550

#define _LANTERN       552
#define _LANTERN_OFF   553

#define _WOODFENCE     554

#define _WOODPOLE      555

#define _WOODDOOR      560
#define _STONEDOOR     561

#define _WOODSIGN      570

#define _GLASS         580 // glass super-transparent objects

#define _CACTUS        600

#define _DIG0          639 // player stuff
#define _DIG1          640
#define _DIG2          641
#define _DIG3          642
#define _DIG4          643
#define _DIG5          644
#define _DIG6          645
#define _DIG7          646
#define _DIG8          647
#define _DIG9          648
#define _SELECTION     649

///  crosses 750->960 ///
#define _FLOWERRED       750
#define _FLOWERYELLOW    751
#define _FLOWERROSE      752
#define _GRASS_LONG      753
#define _GRASS_SHORT     754
#define _DESERTFLOWER    755
#define _FLOWERREDMAG    756
#define _PLANT_DRYBROWN  757
#define _FLOWER_SMALL    758
#define _REEDS           759

#define _BUSH_HOLLY    775
#define _BUSH2         776
#define _BUSH3         777
#define _BUSH4         778
#define _BUSH5         779
#define _BUSH_PINKROSE 780
#define _BUSH_MRSPIKY  781
#define _FLOWER_CROCUS 782

#define _DARKSHROOM    790
#define _REDSHROOM     791
#define _BLUESHROOM    792

#define _WATERLILY     796

#define _IVYLEAF       798
#define _LILYPAD       799

#define _FIRE          800

#define _HANGINGAPPLE  810

#define _TORCH         820

//  underwater crosses 920-959 //
#define _SEAWEED       920

// alphablend blocks (960+) //

#define _WATER        1000  // liquid H20
#define _LAVABLOCK    1001

#define isAir(id) (id == _AIR)
#define isStone(id) (id >= stone_start && id <= stone_end)
#define isDirt(id) (id >= soil_start && id <= soil_end)
#define isSand(id) (id >= sand_start && id <= sand_end)
#define isLeaf(id) (id >= trees_start && id <= trees_end)
#define isCross(id) (id >= cross_start && id <= cross_end)
#define isFluid(id) (id == _WATER || id == _LAVABLOCK)
#define isLight(id) (id == _TORCH || id == _MOLTENSTONE || id == _LAVABLOCK || id == _REDSTONE || id == _GREENSTONE || id == _FIRE || id == _LANTERN)

#endif
