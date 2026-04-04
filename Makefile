# ReXamples root makefile
# Native ReAction lessons for AmigaOS 3.2+
#
# Expected environment:
#   source /path/to/env.sh

LESSONS = \
	01_Window \
	02_Layout \
	03_Button \
	04_CheckBox \
	05_Chooser \
	06_String \
	07_TextEditor \
	08_ListBrowser \
	09_RadioChoice \
	10_Image \
	11_Slider \
	12_Space \
	13_Frame \
	14_Menu_Basic \
	15_Menu_Advanced \
	16_Gauge \
	17_TextDisplay \
	18_ScrollableLayout \
	19_RegisterPages \
	20_EventDispatch

.PHONY: all clean list $(LESSONS)

all: $(LESSONS)

list:
	@printf "%s\n" $(LESSONS)

$(LESSONS):
	$(MAKE) -C $@

clean:
	@for d in $(LESSONS); do \
		$(MAKE) -C $$d clean || exit $$?; \
	done
