![Isologotipo FCEFyN y UNC](./assets/1_1_Isologotipo%20FCEFyN%20y%20UNC-%20original_%20fondo-Con%20bajada.jpg "UNC y FCEFyN")

# TRABAJO PRÁCTICO N° 3  
## Modo Protegido

**Integrantes:**
- García, Lautaro Misael
- Gomez, Dolores
- Renaudo Gaggioli, Valentino

**Abril 2026**

---

## Desafío UEFI

### **UEFI**

UEFI es la especificación que define la interfaz entre el firmware del hardware y el sistema operativo. Es lo primero que se ejecuta al encender la PC, antes que cualquier SO, y reemplaza al viejo BIOS que se usó durante décadas.

El BIOS quedó obsoleto porque operaba en 16 bits, no soportaba discos mayores a 2.2 TB, tenía arranque lento y sin ningún mecanismo de seguridad. UEFI resuelve todo eso: trabaja en 32/64 bits, soporta discos enormes con GPT, paraleliza la inicialización del hardware para arrancar más rápido, implementa Secure Boot para verificar firmas digitales, y tiene soporte nativo de drivers, red e interfaz gráfica.

El proceso de arranque está dividido en fases. SEC usa el cache de la CPU como RAM temporal antes de que la memoria esté disponible. PEI inicializa la RAM y el chipset. DXE carga todos los drivers modulares (USB, disco, GPU, red). BDS elige el dispositivo de arranque y aplica Secure Boot. Finalmente RT mantiene algunos servicios activos mientras el sistema operativo está corriendo.

UEFI expone dos grupos de servicios: los Boot Services, que solo existen antes de que el SO tome el control, y los Runtime Services, que persisten después. El más usado es GetVariable/SetVariable, que permite leer y escribir variables persistentes en la NVRAM como el orden de arranque o el estado de Secure Boot.

### **Vulnerabilidades UEFI**

El firmware UEFI es un objetivo muy valioso para atacantes porque se ejecuta antes del sistema operativo, es invisible para los antivirus, y sobrevive a formateos y reinstalaciones completas.

**LoJax (2018)** fue el primer rootkit UEFI usado en ataques reales, operado por APT28 (GRU ruso). Los atacantes leyeron el contenido de la flash SPI, insertaron un módulo DXE malicioso en la imagen del firmware y la reescribieron. Desde entonces, en cada arranque el módulo se cargaba antes del SO e instalaba malware. Sobrevive incluso al reemplazo físico del disco.

**MosaicRegressor (2020)**, atribuido a un APT norcoreano, hacía algo similar pero insertaba un módulo completamente nuevo en el firmware que descargaba componentes adicionales desde internet al iniciar el sistema.

**CosmicStrand (2022)** afectaba placas ASUS y Gigabyte con chipset H81. En lugar de tocar el disco, modificaba directamente la rutina de transferencia al kernel de Windows para inyectar código malicioso antes de que cualquier mecanismo de seguridad del SO estuviera activo.

**BlackLotus (2023)** fue el primer bootkit vendido públicamente capaz de bypassear Secure Boot en Windows 11 actualizado, explotando CVE-2022-21894. Usaba un gestor de arranque legítimamente firmado pero vulnerable que Secure Boot aceptaba sin problema, y desde ahí deshabilitaba Defender, BitLocker y VBS.

**LogoFAIL (2023)** explotó vulnerabilidades en los parsers de imágenes que UEFI usa para mostrar logos de marca al arrancar. Un atacante podía colocar una imagen maliciosa en la EFI System Partition y lograr ejecución de código antes de que Secure Boot actuara, ya que esa verificación ocurre fuera de su alcance.

### **Intel CSME y MEBx**

El CSME es un microprocesador independiente integrado en el chipset de los sistemas Intel. No tiene nada que ver con el procesador principal: tiene su propia CPU, su propia RAM, su propio sistema operativo embebido y opera incluso cuando el equipo está apagado, siempre que haya tensión de standby. Tiene acceso directo a la red, a la memoria del sistema y al almacenamiento.

Sus funciones principales son Intel Boot Guard (verifica que el firmware UEFI no fue modificado antes de que arranque), Intel AMT (administración remota fuera de banda, incluso con el SO apagado), Intel PTT (TPM 2.0 por software, sin necesidad de chip físico separado) y servicios criptográficos varios.

En cuanto a vulnerabilidades: CVE-2017-5689 permitía autenticarse en la interfaz web de AMT enviando una contraseña vacía, afectando millones de equipos empresariales. CVE-2019-0090 estaba en la ROM de arranque del CSME y no era parcheable por software, permitiendo comprometer toda la cadena de confianza del sistema incluyendo Boot Guard.

El MEBx es la interfaz de configuración del CSME, accesible antes del arranque con Ctrl+P. Permite habilitar o deshabilitar AMT, cambiar su contraseña (que por defecto es "admin"), configurar su red independiente y habilitar acceso KVM remoto. En 2017, F-Secure demostró que con acceso físico de menos de 30 segundos y la contraseña por defecto, se puede establecer una backdoor de acceso remoto permanente que sobrevive a cualquier reinstalación del SO y es invisible para el sistema operativo.

### **Coreboot**

Coreboot es un proyecto de firmware open source bajo licencia GPLv2 cuyo objetivo es reemplazar el firmware propietario con el mínimo de código necesario para inicializar el hardware, y luego pasar el control a un payload modular (GRUB, SeaBIOS, TianoCore, Linux, etc.).

Su arquitectura es simple: bootblock arranca desde la ROM, romstage inicializa la RAM, ramstage detecta y configura el hardware completo, y el payload se encarga del resto.

Las ventajas frente al firmware propietario son concretas. Al ser código abierto, puede auditarse en busca de backdoors o vulnerabilidades. Al ser minimal, tiene menos superficie de ataque. El tiempo de arranque puede bajar a menos de un segundo. Algunas configuraciones permiten neutralizar el CSME de Intel con herramientas como me\_cleaner, eliminando esa superficie de ataque completamente.

En cuanto a adopción comercial: todos los Chromebooks de Google desde 2012 usan Coreboot, lo que lo convierte en el firmware open source más desplegado del mundo. Framework Laptop lo usa en sus modelos AMD. Purism lo incluye junto con Heads para máxima seguridad. System76 está migrando sus laptops a Coreboot bajo el proyecto Open Firmware. Los appliances Protectli Vault también lo usan. Y Facebook/Meta lo implementó en sus servidores del Open Compute Project.

## Desafío Linker
## Desafío Modo Protegido