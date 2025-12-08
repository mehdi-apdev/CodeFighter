import os
import xml.etree.ElementTree as ET

# --- CONFIGURATION ---
CB_PROJECT_NAME = "CodeFighters"
CBP_FILENAME = f"{CB_PROJECT_NAME}.cbp"

# 1. Déterminer les chemins de manière absolue (Indépendant d'où on lance le script)
# Le script est dans /tools, donc la racine est le dossier parent de /tools
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_ROOT = os.path.dirname(SCRIPT_DIR)
CBP_PATH = os.path.join(PROJECT_ROOT, CBP_FILENAME)

# Dossiers à scanner (relatifs à la racine)
SOURCE_DIRS = ['src', 'include']

def generate_cbp_skeleton():
    """Crée un fichier .cbp vierge si absent"""
    content = f"""<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
<CodeBlocks_project_file>
	<FileVersion major="1" minor="6" />
	<Project>
		<Option title="{CB_PROJECT_NAME}" />
		<Option pch_mode="2" />
		<Option compiler="gcc" />
		<Build>
			<Target title="Debug">
				<Option output="bin/Debug/{CB_PROJECT_NAME}" prefix_auto="1" extension_auto="1" />
				<Option object_output="obj/Debug/" />
				<Option type="1" />
				<Option compiler="gcc" />
				<Compiler>
					<Add option="-g" />
					<Add directory="include" />
					<Add directory="$(#sfml.include)" />
				</Compiler>
				<Linker>
					<Add library="sfml-graphics-d" />
					<Add library="sfml-window-d" />
					<Add library="sfml-system-d" />
					<Add library="sfml-audio-d" />
					<Add directory="$(#sfml.lib)" />
				</Linker>
			</Target>
			<Target title="Release">
				<Option output="bin/Release/{CB_PROJECT_NAME}" prefix_auto="1" extension_auto="1" />
				<Option object_output="obj/Release/" />
				<Option type="1" />
				<Option compiler="gcc" />
				<Compiler>
					<Add option="-O2" />
					<Add directory="include" />
					<Add directory="$(#sfml.include)" />
				</Compiler>
				<Linker>
					<Add option="-s" />
					<Add library="sfml-graphics" />
					<Add library="sfml-window" />
					<Add library="sfml-system" />
					<Add library="sfml-audio" />
					<Add directory="$(#sfml.lib)" />
				</Linker>
			</Target>
		</Build>
		<Compiler>
			<Add option="-Wall" />
			<Add option="-std=c++17" />
			<Add option="-fexceptions" />
		</Compiler>
		<Unit filename="main.cpp">
			<Option compilerVar="CC" />
		</Unit>
		<Extensions />
	</Project>
</CodeBlocks_project_file>
"""
    if not os.path.exists(CBP_PATH):
        with open(CBP_PATH, "w") as f:
            f.write(content)
        print(f"📄 Squelette {CBP_FILENAME} créé à la racine.")

def update_cbp():
    # 1. Générer si n'existe pas
    generate_cbp_skeleton()

    # 2. Lire le XML
    try:
        tree = ET.parse(CBP_PATH)
        root = tree.getroot()
        project = root.find('Project')
    except Exception as e:
        print(f"❌ Erreur lecture CBP : {e}")
        return

    # 3. Scanner les fichiers réels sur le disque
    real_files = set()
    for d in SOURCE_DIRS:
        full_dir_path = os.path.join(PROJECT_ROOT, d)
        if os.path.exists(full_dir_path):
            for root_dir, _, files in os.walk(full_dir_path):
                for f in files:
                    if f.endswith(('.cpp', '.hpp', '.h', '.c')):
                        # On calcule le chemin relatif par rapport à la RACINE DU PROJET (où est le .cbp)
                        abs_file_path = os.path.join(root_dir, f)
                        rel_path = os.path.relpath(abs_file_path, PROJECT_ROOT).replace('\\', '/')
                        real_files.add(rel_path)

    # 4. Nettoyer les balises <Unit> existantes
    # (On supprime tout pour reconstruire proprement, c'est plus sûr)
    for unit in project.findall('Unit'):
        project.remove(unit)

    # 5. Réinsérer les fichiers triés
    count = 0
    for f in sorted(list(real_files)):
        unit = ET.SubElement(project, 'Unit')
        unit.set('filename', f)
        # Si c'est un .cpp, on ajoute l'option compilerVar
        if f.endswith(('.cpp', '.c')):
            ET.SubElement(unit, 'Option', {'compilerVar': 'CC'})
        count += 1
    
    # 6. Sauvegarder
    ET.indent(tree, space="\t", level=0)
    tree.write(CBP_PATH, encoding='UTF-8', xml_declaration=True)
    print(f"✅ Code::Blocks Project ({CBP_FILENAME}) mis à jour : {count} fichiers liés.")

if __name__ == "__main__":
    update_cbp()