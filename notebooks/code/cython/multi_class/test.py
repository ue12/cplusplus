import pyclassA
import pyclassB 

a = pyclassA.PyCppA("coucou")
b = pyclassB.PyCppB("byebye")

print(a.getName())
print(b.getName())

a.attach( b )

