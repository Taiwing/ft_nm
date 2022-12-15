# ft\_nm

Implementation of the nm command in C.

## Setup

```shell
# clone it with the libft submodule
git clone --recurse-submodules https://github.com/Taiwing/ft_nm
# build it
cd ft_nm/ && make
# run it
sudo ./ft_nm ft_nm
```

As shown above this program needs sudo rights. If you do not have root access
on your machine but docker is available, then execute the following commands to
run ft\_nm:

```shell
# build docker image and run it
./setup-docker.bash
# run ft_nm inside the container
./ft_nm ft_nm
```
