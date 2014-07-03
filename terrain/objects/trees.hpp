#ifndef TREES_HPP
#define TREES_HPP

extern void otreeSphere(int gx, int gy, int gz);
extern void createTreeOval(int gx, int gy, int gz, int maxheight);
extern void otreeBirch(int gx, int gy, int gz, int height);
extern void otreeSabal(int gx, int gy, int gz, int height);
extern void otreeJungleVines(int gx, int gy, int gz, int height);
extern void otreeHuge(int gx, int gy, int gz, int height);

// jungle
extern void ingenBigDarkTree(int gx, int gy, int gz, int height);
extern void ingenJungleTree(int gx, int gy, int gz, int height);

// winter
extern void createIcePine(int gx, int gy, int gz, int height);
extern void createPine(int gx, int gy, int gz, int height);

#endif
