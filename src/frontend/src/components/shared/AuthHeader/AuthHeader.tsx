import { Flex, Heading, Image, Box } from "@chakra-ui/react";
export const AuthHeader = () => {
  return (
    <Box height="20%" width="100%">
      <Flex
        direction="row"
        bg="orange"
        height="100%"
        alignItems="center"
        justifyContent="space-between"
        paddingX="40px"
      >
        <Heading color="white">eZgrada</Heading>
        <Image
          src="https://fakeimg.pl/600x400/ff0000/ffffff?text=Nema+slike+:("
          alt="Naslovna slika showa"
          objectFit="cover"
          height={"80%"}
        />
      </Flex>
    </Box>
  );
};
