import { Flex, Heading, Image, Box } from "@chakra-ui/react";
import logoImage from "../../../../public/logo.png";
export const AuthHeader = () => {
  console.log(logoImage);
  return (
    <Box height="15%" width="100%">
      <Flex
        direction="row"
        bg="orange.400"
        height="100%"
        alignItems="center"
        justifyContent="space-between"
        paddingX="40px"
      >
        <Heading color="white" fontSize="xx-large">
          eZgrada
        </Heading>
        <Image
          src={logoImage.src}
          alt="Naslovna slika showa"
          objectFit="cover"
          height={"80%"}
        />
      </Flex>
    </Box>
  );
};
