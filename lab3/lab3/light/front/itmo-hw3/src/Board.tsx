import React,{Component} from 'react'
import Typography from '@material-ui/core/Typography';
import Card from '@material-ui/core/Card';
import CardContent from '@material-ui/core/CardContent';



class Board extends Component<any,any>{
    render(){
        return(
        <Card style={{width:'60ch',margin:'15px'}}>
            <CardContent>
                <Typography variant="h6">
                Author: {this.props.author}
                </Typography>
                <Typography>
                Comment: {this.props.comment}
                </Typography>
            </CardContent>
        </Card>
        )
    }
}

export default Board;