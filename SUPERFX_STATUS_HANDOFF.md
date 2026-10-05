# Diagnóstico y Estado Actual del Super FX para Yoshi's Island (PSP Go)

## 1. Estado del Proyecto (Último avance)
- **Hito alcanzado:** El emulador arrancó en hardware real PSP Go. Mostró el logo de Nintendo (parte limpia y parte con artefactos) y luego se congeló al iniciar la escena de Yoshi's Island.
- **Razón del congelamiento / artefactos actuales:**
  El pipeline de saltos y prefetch de instrucciones del Super FX (`GSU.vPipe` y `FETCHPIPE`) se mejoró, pero el despachador monolítico aún tiene instrucciones críticas o modos `ALT1/ALT2/ALT3` que entran en ciclo infinito o fallan en la sincronización de caché (`fx_cache` / `CBR`) y registros de pantalla (`vPlotOptionReg`, `CMODE`, `COLOR`, `PLOT`).

## 2. Recomendación Técnica para OpenCode / Warp
Dado que tu meta firme es **100% full speed y emulación limpia sin importar el tiempo**, continuar parcheando el despachador opcode por opcode en un codebase legacy de Snes9x 2005 tiene rendimientos decrecientes. 

Tienes dos caminos estratégicos con OpenCode + Warp:

### Opción 1: Re-arquitectura Limpia (Recompiler Dynarec o Core Moderno)
- **Base recomendada:** Portar o adaptar el core de Super FX moderno de Snes9x moderno (o snes9x-next / Libretro) o snes9xTYL ME limpio.
- **Dynarec Super FX a MIPS:** En lugar de un despachador por saltos indirectos (`jump table` interpretada), implementar un recompilador JIT en bloques básicos que traduzca bloques Super FX a MIPS nativo en memoria de ejecución (scratchpad/RAM de PSP).

### Opción 2: Corregir y completar el fallback limpio en el core actual
Si quieres ver el juego corriendo en el core actual antes de rehacerlo:
1. **Fallback 100% C++ seguro:** Para cualquier opcode donde haya duda (o durante depuración), dejar que la tabla C++ probada de Snes9x original ejecute la instrucción y validar que `GSU.vCounter`, `GSU.vPipe` y `R15` no se desfasen.
2. **Revisar `fx_cache` (Opcode 0x02) y `LJMP` / `JMP`:** Yoshi's Island usa intensivamente el buffer de caché interno (512 bytes de RAM interna en 0x3100). Si la caché no invalida correctamente en MIPS, el chip ejecuta código basura y se traba.
3. **Mapeo de registros R14 (ROM buffer) y R15 (PC):** Cada vez que se toca R14 o R15, la sincronización con el bus SNES debe ser inmediata.

---
*Archivo generado para dar continuidad en nuevas sesiones o herramientas externas (OpenCode).*
