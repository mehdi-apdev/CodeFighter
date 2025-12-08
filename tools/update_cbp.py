import os
import xml.etree.ElementTree as ET

# --- CONFIGURATION ---
PROJECT_FILE = 'CodeFighters.cbp'  # Nom de ton fichier projet
SOURCE_DIRS = ['src', 'include']   # Dossiers à scanner

def generate_cbp_skeleton():
    """Crée un fichier .cbp vierge mais configuré pour SFML si absent"""
    content = """<?xml version="1.0" encoding="UTF-8" standalone="yes" ?>
<CodeBlocks_project_file>
	<FileVersion major="1" minor="6" />
	<Project>
		<Option title="CodeFighters" />
		<Option pch_mode="2" />
		<Option compiler="gcc" />
		<Build>
			<Target title="Debug">
				<Option output="bin/Debug/CodeFighters" prefix_auto="1" extension_auto="1" />
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
				<Option output="bin/Release/CodeFighters" prefix_auto="1" extension_auto="1" />
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
    if not os.path.exists(f"../{PROJECT_FILE}"):
        with open(f"../{PROJECT_FILE}", "w") as f:
            f.write(content)
        print("✅ Fichier .cbp généré (Squelette).")

def update_cbp():
    cbp_path = f"../{PROJECT_FILE}"
    if not os.path.exists(cbp_path):
        generate_cbp_skeleton()

    tree = ET.parse(cbp_path)
    root = tree.getroot()
    project = root.find('Project')

    # Récupérer les fichiers réels
    real_files = set()
    for d in SOURCE_DIRS:
        if os.path.exists(f"../{d}"):
            for root_dir, _, files in os.walk(f"../{d}"):
                for f in files:
                    if f.endswith(('.cpp', '.hpp', '.h')):
                        # Chemin relatif propre (ex: src/Main.cpp)
                        rel_path = os.path.relpath(os.path.join(root_dir, f), '..').replace('\\', '/')
                        real_files.add(rel_path)

    # Nettoyer les Unit existantes
    for unit in project.findall('Unit'):
        project.remove(unit)

    # Ajouter les fichiers
    for f in sorted(list(real_files)):
        unit = ET.SubElement(project, 'Unit')
        unit.set('filename', f)
        if f.endswith('.cpp'):
            ET.SubElement(unit, 'Option', {'compilerVar': 'CC'})
    
    ET.indent(tree, space="\t", level=0)
    tree.write(cbp_path, encoding='UTF-8', xml_declaration=True)
    print(f"✅ Code::Blocks Project mis à jour avec {len(real_files)} fichiers.")

if __name__ == "__main__":
    update_cbp()