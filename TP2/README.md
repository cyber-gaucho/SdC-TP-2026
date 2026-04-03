# TP2 - Stackframe

## Flujo de trabajo con Git y GitHub

Este documento define las reglas y el flujo de trabajo que se deben seguir para el desarrollo del Trabajo Práctico 2 (Stackframe).

---

# 1. Estructura del repositorio

```
/TP2
  /stackframe
    codigo/
    docs/
```

---

# 2. Modelo de trabajo

* Todos los integrantes son colaboradores del repositorio original.
* Se trabaja mediante **ramas** y **Pull Requests (PR)**.
* No se permite trabajar directamente sobre `main` ni `dev-tp2`.

---

# 3. Ramas

## Ramas principales

* `main` → versión estable
* `dev-tp2` → integración del TP2

## Ramas de trabajo

Formato obligatorio:

```
tp2/<nombre>-<feature>
```

Ejemplos:

```
tp2/renaudo-stackframe-base
tp2/garcia-debug
tp2/gomez-tests
```

---

# 4. Flujo de trabajo

## 4.1. Antes de empezar a trabajar

```bash
git checkout main
git pull origin main

git checkout dev-tp2
git pull origin dev-tp2
```

---

## 4.2. Crear rama de trabajo

```bash
git checkout -b tp2/tu-nombre-feature
```

---

## 4.3. Trabajar y commitear

```bash
git add .
git commit -m "feat(tp2): descripción clara del cambio"
git push origin tp2/tu-nombre-feature
```

---

## 4.4. Crear Pull Request

* Crear PR hacia: `dev-tp2`
* No hacia `main`

---

## 4.5. Actualizar rama con cambios nuevos

```bash
git fetch origin
git rebase origin/dev-tp2
```

---

# 5. Reglas de Pull Requests

* Todo cambio debe hacerse mediante PR
* Se requiere **al menos 1 aprobación**
* El autor del PR **no puede aprobar su propio PR**
* Se deben resolver todos los comentarios antes de mergear
* No se permite merge directo a ninguna rama protegida

---

# 6. Reglas de commits

Formato recomendado:

```
feat: nueva funcionalidad
fix: corrección de errores
docs: documentación
refactor: mejora sin cambiar funcionalidad
```

Ejemplo:

```
feat(tp2): implementación inicial del stackframe
```

---

# 7. Buenas prácticas

* Hacer commits pequeños y claros
* No mezclar múltiples funcionalidades en una misma rama
* Actualizar la rama frecuentemente (`rebase`)
* Probar el código antes de abrir un PR
* Escribir código legible y consistente

---

# 8. Cosas que NO se deben hacer

* ❌ Push directo a `main`
* ❌ Push directo a `dev-tp2`
* ❌ Aprobar tu propio PR
* ❌ Subir código que no compila o rompe el proyecto
* ❌ Crear ramas sin seguir la convención

---

# 9. Protección de ramas

Las ramas `main` y `dev-tp2` tienen configuradas las siguientes restricciones:

* Requieren Pull Request
* Requieren al menos 1 aprobación
* No permiten merge sin revisión
* Se deben resolver todas las conversaciones

---

# 10. Cheatsheet Markdown

> **[Full-Markdown](https://gist.github.com/allysonsilva/85fff14a22bbdf55485be947566cc09e)**

---

# 11. Notas finales

* `dev-tp2` es la rama donde se integra todo el TP
* `main` solo recibe código estable y final
* Ante dudas, consultar antes de hacer cambios grandes
