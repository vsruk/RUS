import { Button, Flex, Heading, Image } from "@chakra-ui/react";

export const HomeHeader = () => {
  return (
    <>
      <Flex
        direction="row"
        height="100%"
        alignItems="center"
        width="100%"
        justifyContent="space-between"
        px="20px"
      >
        <Flex direction="row" height="100%" gapX="10px" alignItems="center">
          <Image
            src="https://fakeimg.pl/600x400/ff0000/ffffff?text=Nema+slike+:("
            alt="Naslovna slika showa"
            objectFit="cover"
            height="80%"
          />
          <Heading color="orange" fontSize="3xl">
            eZgrada
          </Heading>
        </Flex>
        <Flex direction="row">
          <Button borderRadius="full" bg="orange" height="30px">
            Log In
          </Button>
        </Flex>
      </Flex>
    </>
  );
};
