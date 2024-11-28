d_cur := $(CURDIR)

ifdef MOD_REL_DIR
rel_path := $(MOD_REL_DIR)

else

## for windows start
# \ --> /
mod_abs_path1 = $(subst \,/,$(MOD_ABS_DIR))
mod_abs_path := $(mod_abs_path1)
# F:/sss --> /sss
d_mod_abs_path := $(shell echo $(mod_abs_path) | sed 's#.*\:##')
# /cygdrive/f/sss --> /sss
d_mod_abs_path := $(shell echo $(d_mod_abs_path) | sed 's#/cygdrive/./#/#')

d_cur := $(shell echo $(d_cur) | sed 's#.*\:##')
d_cur := $(shell echo $(d_cur) | sed 's#/cygdrive/./#/#')
## for windows end

ifneq ($(d_cur),$(d_mod_abs_path))
# /aa, /aa/ss --> ss
rel_path := $(patsubst $(d_cur)/%,%,$(d_mod_abs_path))
endif

endif

# $(info rel_path: $(rel_path))

-include $(rel_path)/mod.mk
src += $(wildcard ${rel_path}/*.c ${rel_path}/*.cpp ${rel_path}/*.cc)
# $(info src: ${src})
