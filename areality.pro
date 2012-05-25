#-------------------------------------------------
#
# Project created by QtCreator 2012-02-17T10:06:10
#
#-------------------------------------------------

TEMPLATE = subdirs
SUBDIRS += artoolkit app 
app.depends=artoolkit

TRANSLATIONS += areality_fi.ts areality_en.ts
OTHER_FILES += areality_fi.ts areality_en.ts
OTHER_FILES += style.qss README
