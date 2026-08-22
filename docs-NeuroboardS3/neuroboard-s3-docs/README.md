# NeuroBoard-S3 — site Marque

Projet de contenu : `marque.toml`, `navigation.mq`, `pages/`, `static/`.
Les dossiers `directives/`, `layouts/`, `themes/` et `common.css` viennent du
template Marque : générez d'abord le squelette, puis copiez ces fichiers par-dessus.

```sh
marque new neuroboard-s3 layout:sidebar theme:comte
# copier marque.toml, navigation.mq, pages/ et static/ par-dessus
cd neuroboard-s3
marque serve .
```

## Structure

| Section | Pages |
| --- | --- |
| — | `index.mq`, `getting-started.mq` |
| Matériel | `hardware.mq`, `pinout.mq` |
| Périphériques | `camera.mq`, `rgb-led.mq`, `microsd.mq`, `boot-button.mq` |
| Frameworks | `frameworks.mq`, `arduino.mq`, `esp-idf.mq` |
| Exemples | `adc-dma.mq`, `buffering.mq`, `fir.mq`, `mfcc.mq` |
| Référence | `constraints.mq`, `resources.mq`, `links.mq` |

Les chemins sont plats et le regroupement vient des titres de section de
`navigation.mq`. Marque déduit l'imbrication du *chemin* des pages : mettre
`camera.mq` dans `pages/peripherals/` créerait un second niveau redondant
avec le titre « Périphériques ».

## Pages incomplètes, à dessein

`fir.mq` et `mfcc.mq` posent les contraintes de la carte mais ne contiennent
pas d'implémentation, le filtrage étant l'objet du TP 8. Chacune se termine par
un `@dropdown` listant ce qui reste à décider.

## Images

Voir `static/images/PLACEHOLDERS.md`.
