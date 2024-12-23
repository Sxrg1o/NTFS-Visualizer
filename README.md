# Desarrollo de un Visualizador Educativo de Sistema de Archivos NTFS para la Enseñanza de Sistemas Operativos

## PROBLEMÁTICA:
El estudio y comprensión del sistema de archivos NTFS presenta varios desafíos significativos en el contexto educativo:

- Complejidad Conceptual:
NTFS posee una estructura interna compleja con múltiples componentes interrelacionados (MFT, clusters, atributos, journaling, etc.)
Los conceptos son altamente abstractos y difíciles de visualizar mentalmente
La documentación técnica suele ser densa y poco accesible para estudiantes

- Brecha entre Teoría y Práctica:
Los materiales educativos tradicionales (libros, diapositivas) son limitados para explicar estructuras dinámicas
Existe una carencia de herramientas prácticas que permitan a los estudiantes explorar y manipular las estructuras NTFS de manera segura
Los estudiantes tienen pocas oportunidades de experimentar con casos reales sin riesgo de dañar sus sistemas

- Limitaciones Pedagógicas:
Los docentes carecen de recursos didácticos interactivos para demostrar conceptos complejos
Es difícil mostrar la relación entre diferentes componentes del sistema de archivos
No hay forma práctica de visualizar cómo los cambios en un nivel afectan a otros niveles del sistema

- Accesibilidad Técnica:
Las herramientas existentes para análisis de NTFS son principalmente forenses o técnicas, no educativas
Muchas herramientas son específicas de plataforma (solo Windows o solo Linux)
La mayoría requiere privilegios de administrador o conocimientos técnicos avanzados

- Necesidades Educativas No Cubiertas:
Los estudiantes necesitan una forma práctica de validar su comprensión teórica
Falta un medio para explorar la estructura NTFS de manera guiada y progresiva
No existen suficientes recursos para autoaprendizaje y experimentación

## OBJETIVOS: 

### OBJETIVO GENERAL:
Desarrollar una herramienta educativa multiplataforma que permita visualizar y comprender la estructura y funcionamiento del sistema de archivos NTFS mediante representaciones gráficas interactivas, facilitando el proceso de enseñanza-aprendizaje en cursos de sistemas operativos a nivel universitario.

### OBJETIVOS ESPECÍFICOS:

1. Diseñar e implementar una interfaz gráfica que represente la estructura jerárquica del sistema de archivos NTFS:
  - Crear visualizaciones claras de la Master File Table (MFT)
  - Desarrollar representaciones gráficas de la estructura de clusters y sectores
  - Implementar la visualización de metadatos y atributos de archivos
  - Diseñar una navegación intuitiva entre diferentes niveles de la estructura

2. Implementar funcionalidades de análisis y exploración de componentes NTFS:
  - Desarrollar mecanismos para la lectura segura de estructuras NTFS
  - Crear funciones para el análisis de atributos de archivos y directorios
  - Implementar herramientas para la exploración de la MFT
  - Desarrollar capacidades de búsqueda y filtrado de elementos

3. Crear módulos educativos interactivos:
  - Diseñar tutoriales guiados sobre conceptos fundamentales de NTFS
  - Desarrollar ejercicios prácticos con retroalimentación inmediata
  - Implementar casos de estudio interactivos
  - Crear simulaciones de operaciones comunes en NTFS

4. Validar la efectividad de la herramienta en un entorno educativo:
  - Diseñar instrumentos de evaluación para medir el impacto educativo
  - Implementar la herramienta en un curso piloto
  - Recopilar y analizar feedback de estudiantes y docentes
  - Realizar ajustes basados en los resultados obtenidos

5. Documentar el desarrollo y uso de la herramienta:
  - Crear manuales de usuario para estudiantes y docentes
  - Desarrollar guías de instalación y configuración
  - Elaborar material de apoyo pedagógico
  - Documentar la arquitectura y decisiones técnicas del sistema

6. Garantizar la portabilidad y accesibilidad:
  - Implementar soporte multiplataforma (Windows y Linux)
  - Optimizar el rendimiento en diferentes entornos
  - Minimizar requisitos de instalación y configuración
  - Asegurar la compatibilidad con diferentes versiones de NTFS

## MARCO TEÓRICO: 

  1. Fundamentos de Sistemas de Archivos
  1.1. Conceptos básicos:
  
    - Estructura jerárquica de directorios
    - Gestión de espacio en disco
    - Sistemas de archivos FAT vs NTFS
    - Metadatos y atributos
    - Fragmentación y desfragmentación

  2. NTFS (New Technology File System)
  2.1. Historia y Evolución:
  
    - Desarrollo por Microsoft
    - Versiones y mejoras
    - Comparación con otros sistemas de archivos
  
  2.1. Estructura Fundamental:
  
    - Master File Table (MFT)
      - Registros MFT
      - Estructura de entradas
      - Metadatos y atributos
  
    - Clusters y Sectores:
      - Tamaño de cluster
      - Organización física vs lógica
      - Gestión de espacio
  
    - Sistema de Atributos:
      - Atributos residentes vs no residentes
      - $DATA
      - $FILE_NAME
      - $STANDARD_INFORMATION
      - Otros atributos importantes
  
  2.3. Características Avanzadas:
  
    - Journaling (registro de transacciones)
    - Compresión y cifrado
    - Cuotas de disco
    - Puntos de montaje y enlaces
    - Permisos y seguridad
  
  3. Aspectos Pedagógicos
  3.1. Teorías de Aprendizaje:
  
    - Aprendizaje constructivista
    - Aprendizaje basado en problemas
    - Visualización de datos en educación
  
  3.2. Estrategias Didácticas:
  
    - Métodos interactivos
    - Retroalimentación inmediata
    - Progresión pedagógica
    
  4. Visualización de Datos
  
    - Principios de Visualización:
      - Representación jerárquica
      - Navegación interactiva
      - Diseño de interfaces educativas
