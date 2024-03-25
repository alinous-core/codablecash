
set(__src
	AbstractMerkleElement.cpp
	BlankMerkleElement.cpp
	MarkleElementFifo.cpp
	MerkleCertificate.cpp
	MerkleCertificateElement.cpp
	MerkleElement.cpp
	MerkleTree.cpp
)
handle_sub(codablecashlib "${__src}" blockchain merkletree)
