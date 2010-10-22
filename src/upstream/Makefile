.PHONY: upstream
upstream: qtsolutions

.PHONY: qtsolutions qtcolortriangle qtpropertybrowser qtwindowlistmenu
qtsolutions: qtcolortriangle qtpropertybrowser qtwindowlistmenu

qtcolortriangle:
	@cd QtSolutions/qtcolortriangle-2.6_1-opensource ; \
	echo "license accepted" > .licenseAccepted ; \
	./configure

qtpropertybrowser:
	@cd QtSolutions/qtpropertybrowser-2.5_1-opensource ; \
	echo "license accepted" > .licenseAccepted ; \
	./configure

qtwindowlistmenu:
	@cd QtSolutions/qtwindowlistmenu-2.2_1-opensource ; \
	echo "license accepted" > .licenseAccepted ; \
	./configure

qtlockedfile:
	@cd QtSolutions/qtlockedfile-2.4_1-opensource ; \
	echo "license accepted" > .licenseAccepted ; \
	./configure

qtsingleapplication:
	@cd QtSolutions/qtsingleapplication-2.6_1-opensource ; \
	echo "license accepted" > .licenseAccepted ; \
	./configure
