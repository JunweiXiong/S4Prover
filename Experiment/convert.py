import os

for root, subdirs, files in os.walk(os.getcwd()):
    for file in files:
        if file.endswith(".intohylo") and not file.startswith("._"):
            with open(root + "/"+file, "r") as f:
                print(root+file)
                text = f.readlines()[0]
                text = text.replace("-", "~")
                text = text.replace("box", "[]") 
                text = text.replace("dia", "<>") 
                text = text.replace(";", "|") 
                text = text.replace("v", "|") 
                text = text.replace("p", "p1") 
                text = text.replace("q", "p2") 
                text = text.replace("r", "p3") 
                with open("/home/junwei/S4Prover/benchmarks/S4fml/"+file.replace(".intohylo", ".hf"), "w") as f:
                    f.write(text)
                text = text.replace("true", "$true")
                text = text.replace("false", "$false")
                with open("/home/junwei/S4Prover/benchmarks/S4fml/"+file.replace(".intohylo", ".cf"), "w") as f:
                    f.write(text)
