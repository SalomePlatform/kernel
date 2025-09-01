Procédure d'installation du package salome_kernel
=================================================

Prérequis
---------

Avoir le dernier tag Git au format X.Y.Z

Procédure
---------

* Positionner l'environnement de construction
  - soit en positionnant les variables d'environnement cherchées par cmake (xxx_ROOT_DIR)
  - soit en indiquant ces variables lors de l'installation
  - les variables attendues sont :
    - OMNIORBPY_DIR
    - LIBBATCH_ROOT_DIR
    - CMAKE_MODULE_PATH

* Créer un environnement virtuel python
  - python3 -m venv .venv

* Copier le script 'activate' dans le répertoire 'bin' du venv
  - cp -f activate .venv/bin

* Activer le venv et mettre à jour pip et setuptools. Le package build peut aussi être installé pour la construction du wheel.
  - source .venv/bin/activate
  - python3 -m pip install -U pip setuptools build

* Pour construire et installer le wheel de salome_kernel (par défaut les fichiers .tar.gz et .whl sont produits dans le répertoire dist)
  - python3 -m build install requirements.txt
  - python3 -m build --no-isolation -v
  - python3 -m pip install dist/salome_kernel-[XXX].whl

* Pour installer salome_kernel sans conserver le wheel
  - python3 -m pip install .

* Il est possible de fournir des options aux commandes pip pour configurer cmake :
  * Par une option à pip :
    - pip install -Ccmake.define.CMAKE_BUILD_PARALLEL_LEVEL=8 .
  * Avec une variable d'environnement :
    - CMAKE_BUILD_PARALLEL_LEVEL=8 pip install .

Attention : le wheel produit n'est pas distribuable en l'état. Une réparation est nécessaire.
Cf https://scikit-build-core.readthedocs.io/en/stable/build.html#repairing
