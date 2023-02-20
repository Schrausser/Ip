@echo off

rem änderung des kommazeichens zur weiteren verarbeitung in excel

azube Ip_PSG_linVekt.asc Ip_PSG_linVekt.as_ . , > nil
azube Ip_PSG_linMin.asc Ip_PSG_linMin.as_ . , > nil
azube Ip_PSG_linMax.asc Ip_PSG_linMax.as_ . , > nil
azube Ip_PSG_InfMin.asc Ip_PSG_InfMin.as_ . , > nil
azube Ip_PSG_InfMax.asc Ip_PSG_InfMax.as_ . , > nil
azube Ip_PSG_psgMin.asc Ip_PSG_psgMin.as_ . , > nil
azube Ip_PSG_psgMax.asc Ip_PSG_psgMax.as_ . , > nil

del Ip_PSG_linVekt.asc 
del Ip_PSG_linMin.asc
del Ip_PSG_linMax.asc 
del Ip_PSG_InfMin.asc 
del Ip_PSG_InfMax.asc 
del Ip_PSG_psgMin.asc 
del Ip_PSG_psgMax.asc

ren Ip_PSG_linVekt.as_ Ip_PSG_linVekt.asc 
ren Ip_PSG_linMin.as_ Ip_PSG_linMin.asc 
ren Ip_PSG_linMax.as_ Ip_PSG_linMax.asc
ren Ip_PSG_InfMin.as_ Ip_PSG_InfMin.asc
ren Ip_PSG_InfMax.as_ Ip_PSG_InfMax.asc
ren Ip_PSG_psgMin.as_ Ip_PSG_psgMin.asc
ren Ip_PSG_psgMax.as_ Ip_PSG_psgMax.asc