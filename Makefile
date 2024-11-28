
# 编译运行
#
# 用法：
# 		make MOD_SRC_PATH={包含main函数的单独源文件的路径} {目标}
#        -- 相对路径，相对本文件所在目录
# 	或者
# 		make MOD_REL_DIR={包含mod.mk的相对路径} {目标}
#        -- 相对路径，相对本文件所在目录
# 	或者
# 		make MOD_ABS_DIR={包含mod.mk的绝对路径} {目标}
#
# 举例：
#		make MOD_SRC_PATH=button/button.c run
#		make MOD_SRC_PATH=button/button.cpp run
#		make MOD_REL_DIR=button run
#		make MOD_ABS_DIR=/cygdrive/f/raylib/button run
#		make MOD_ABS_DIR=f:/raylib/button run
#		make MOD_ABS_DIR=F:/raylib/button run


ifdef MOD_SRC_PATH
SRC := $(subst \,/,$(MOD_SRC_PATH))
rel_path := $(dir $(SRC))

else
include base.mk
SRC += $(src)

endif
# $(info SRC: ${SRC})


LIB_DIR += lib

INC_DIR += inc \
animation \
button

TMP_DIR := build

EXE := $(TMP_DIR)/run.exe

CROSS_COMPILE :=
CC		:= $(CROSS_COMPILE)gcc
CPP		:= $(CC) -E
LD		:= $(CROSS_COMPILE)g++

# 生成 预处理文件和汇编文件
# CPPFLAGS += -save-temps -dumpdir $$(dirname $@)/

CPPFLAGS += -g
CPPFLAGS += -DDBG_ON
CPPFLAGS += -Wall -Wextra -MMD
CPPFLAGS += $(addprefix -I,$(INC_DIR))


CFLAGS += -std=c2x
CFLAGS += $(CPPFLAGS)

CXXFLAGS += -std=c++23
CXXFLAGS += $(CPPFLAGS)


LDFLAGS += -lraylib
# Windows 下需要打开
LDFLAGS += -static
LDFLAGS += -lopengl32 -lgdi32 -lwinmm
# 避免产生控制台窗口
LDFLAGS += -Wl,--subsystem,windows
# Windows 下需要打开 结束
LDFLAGS += $(addprefix -L,$(LIB_DIR))

# 源文件转换到temp路径
TMP_FILES := $(addprefix $(TMP_DIR)/,$(SRC))
# .o对象文件生成
OBJS := $(TMP_FILES:.c=.o)
OBJS := $(OBJS:.cpp=.o)
OBJS := $(OBJS:.cc=.o)
# $(info OBJS: ${OBJS})

# .d依赖文件生成
DEPS := $(OBJS:.o=.d)
# CPPS := $(OBJS:.o=.ii)
# DASMS := $(OBJS:.o=.s)

RM := rm -rf
MKDIR := mkdir -p

.PHNOY: all run clean distclean rebuild

all: compile

compile: $(EXE)

run: $(EXE)
	@echo
	export LD_LIBRARY_PATH=$(CURDIR)/$(LIB_DIR); $<

clean:
	$(RM) $(OBJS) $(DEPS) $(CPPS) $(DASMS)

distclean: clean
	$(RM) $(EXE)  $(TMP_DIR)/$(rel_path)

rebuild: clean compile


# 生成可执行程序
$(EXE) : $(OBJS)
	@echo "LD $@"
	@$(MKDIR) $(@D)
	$(LD) -o $@ $^ $(LDFLAGS)

$(TMP_DIR)/%.o: %.c
	@echo "CC $@"
	@$(MKDIR) $(@D)
	@$(CC) -o $@ -c $(CFLAGS) $<

$(TMP_DIR)/%.o: %.cpp
	@echo "CC $@"
	@$(MKDIR) $(@D)
	@$(CC) -o $@ -c $(CXXFLAGS) $<

$(TMP_DIR)/%.o: %.cc
	@echo "CC $@"
	@$(MKDIR) $(@D)
	@$(CC) -o $@ -c $(CXXFLAGS) $<

-include $(DEPS)
